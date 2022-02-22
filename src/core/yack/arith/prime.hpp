//! \file

#ifndef YACK_ARITH_PRIME_INCLUDED
#define YACK_ARITH_PRIME_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace core
    {
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

            static const uint8_t  p8[53];
            static const size_t   n8 = sizeof(p8)/sizeof(p8[0]);
            static const uint16_t p16[6488];
            static const size_t   n16 = sizeof(p16)/sizeof(p16[0]);

        };



    }

    //! prime for 64bits unsigned
    struct prime64
    {
        static bool     is(const uint64_t n)   throw(); //!< check is prime
        static uint64_t next(const uint64_t n) throw(); //!< get next prime


    };



}

#endif

