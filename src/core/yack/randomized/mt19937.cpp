
#include "yack/randomized/mt19937.hpp"
#include "yack/system/seed.hpp"

namespace yack
{

    namespace randomized
    {
        mt19937:: ~mt19937() throw()
        {
        }

        mt19937:: mt19937(const unsigned long s) throw() :
        randomized::bits(0xffffffff),
        mti(N+1),
        mt()
        {
            init_genrand(s<=0? system_seed::get<unsigned long>() : s);
        }

        mt19937:: mt19937(const unsigned long init_key[], const size_t key_length) throw() :
        randomized::bits(0xffffffff),
        mti(N+1),
        mt()
        {
            assert(yack_good(init_key,key_length));
            init_by_array(init_key,key_length);
        }

        /* initializes mt[N] with a seed */
        void mt19937:: init_genrand(const unsigned long s) throw()
        {
            mt[0]= s & 0xffffffffUL;
            for (mti=1; mti<N; mti++) {
                mt[mti] =
                (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
                /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
                /* In the previous versions, MSBs of the seed affect   */
                /* only MSBs of the array mt[].                        */
                /* 2002/01/09 modified by Makoto Matsumoto             */
                mt[mti] &= 0xffffffffUL;
                /* for >32 bit machines */
            }
        }

        /* initialize by an array with array-length */
        /* init_key is the array for initializing keys */
        /* key_length is its length */
        /* slight change for C++, 2004/2/26 */
        void mt19937:: init_by_array(const unsigned long init_key[], const size_t key_length) throw()
        {
            assert(yack_good(init_key,key_length));
            size_t i, j, k;
            init_genrand(19650218UL);
            i=1; j=0;
            k = (N>key_length ? N : key_length);
            for (; k; k--) {
                mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
                + init_key[j] + j; /* non linear */
                mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++; j++;
                if (i>=N) { mt[0] = mt[N-1]; i=1; }
                if (j>=key_length) j=0;
            }
            for (k=N-1; k; k--) {
                mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
                - i; /* non linear */
                mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++;
                if (i>=N) { mt[0] = mt[N-1]; i=1; }
            }

            mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
        }


        /* generates a random number on [0,0xffffffff]-interval */
        uint32_t mt19937:: next32(void) throw()
        {
            unsigned long y;
            static unsigned long mag01[2]={0x0UL, MATRIX_A};
            /* mag01[x] = x * MATRIX_A  for x=0,1 */

            if (mti >= N) { /* generate N words at one time */
                size_t kk;

                if (mti == N+1)   /* if init_genrand() has not been called, */
                    init_genrand(5489UL); /* a default initial seed is used */

                for (kk=0;kk<N-M;kk++) {
                    y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                    mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                for (;kk<N-1;kk++) {
                    y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                    mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
                mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

                mti = 0;
            }

            y = mt[mti++];

            /* Tempering */
            y ^= (y >> 11);
            y ^= (y << 7) & 0x9d2c5680UL;
            y ^= (y << 15) & 0xefc60000UL;
            y ^= (y >> 18);

            return y;
        }

    }

}


