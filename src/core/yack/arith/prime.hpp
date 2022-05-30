//! \file

#ifndef YACK_ARITH_PRIME_INCLUDED
#define YACK_ARITH_PRIME_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace core
    {
        //! basic function for primality
        struct primes
        {
            //! generic is_prime function
            template <typename T> inline
            static bool check(const T n)
            {
                if( n == 2 || n == 3)
                    return true;

                if( n <= 1 || (n % 2) == 0 || (n % 3) == 0)
                    return false;

                for(T i = 5; i*i <= n; i += 6)
                {
                    if ( (n%i) == 0 || (n % (i + 2)) == 0)
                        return false;
                }

                return true;
            }

            static const uint8_t  p8[54];                           //!< 8bits primes
            static const size_t   n8 = sizeof(p8)/sizeof(p8[0]);    //!< number of 8bits primes
            static const uint16_t p16[6488];                        //!< 16 bit primes
            static const size_t   n16 = sizeof(p16)/sizeof(p16[0]); //!< number of 16 bits primes
            static const uint16_t h16 = 0xfff1;                     //!< highest 16 bits primes

            static const size_t pi8   = n8;     //!< PrimePi(8)
            static const size_t pi16  = n8+n16; //!< PrimePi(16)

            static uint64_t     get16(const size_t indx) throw(); //! indx in [0:PrimePi(16)-1]
        };



    }

    //! prime for 64bits unsigned
    struct prime64
    {
        static bool     is(const uint64_t n) throw(); //!< check is prime
        static uint64_t next(uint64_t n)     throw(); //!< get next prime
    };



}

#endif

