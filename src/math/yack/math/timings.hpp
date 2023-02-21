//! \file

#ifndef YACK_TIMINGS_INCLUDED
#define YACK_TIMINGS_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        //! helper to compute timings and related value
        struct timings
        {
            //! f * 10^e <= x
            template <typename T> static inline
            T round_floor(const T a) noexcept
            {
                assert(a>0);
                static const T ten(10);
                const        T a_log = floor( log10(a) );
                const        T a_one = floor( a * pow( ten,-a_log));
                return a_one * pow(ten,a_log);
                
            }

            //! f * 10^e >= x
            template <typename T> static inline
            T round_ceil( const T a ) noexcept
            {
                assert( a > 0 );
                static const T ten(10);
                const        T a_log = floor( log10(a) );
                const        T a_one = ceil( a * pow( ten,-a_log));
                return  a_one * pow(ten,a_log);
            }

        };
    }

}

#endif
