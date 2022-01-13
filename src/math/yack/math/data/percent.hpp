//! \file

#ifndef YACK_MATH_PERCENT_INCLUDED
#define YACK_MATH_PERCENT_INCLUDED 1

#include "yack/arith/ipower.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        //! computing percentage
        struct percent
        {
            //! with significant digits
            template <typename T,const size_t DIGITS=2> static inline
            T of(const T x, const T dx) throw()
            {
                static const T tiny(1e-30);
                static const T hundred(100);
                static const T half(0.5);
                static const T expand = ipower<T,unsigned>(10,DIGITS);

                const T abs_x  = fabs(x);
                const T abs_dx = fabs(dx);
                T       ratio  = 1;
                if(abs_dx<abs_x)
                {
                    ratio = abs_dx/(abs_x+tiny);
                }
                return floor( hundred * ratio * expand + half ) / expand;
            }
        };

    }
}

#endif

