//! \file

#ifndef YACK_MATH_HYPOTHENUSE_INCLUDED
#define YACK_MATH_HYPOTHENUSE_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace math
    {

        //! precise hypothenuse of [extended]float|double|long double
        template <typename T> inline
        T hypothenuse(const T &a, const T &b)
        {
            static const T one  = 1;
            static const T zero = 0;
            const T        absa = abs_of(a);
            const T        absb = abs_of(b);
            if(absa>absb)
            {
                const T args = one+squared(absb/absa);
                return  absa * sqrt_of(args);
            }
            else
            {
                if(absb<=zero)
                {
                    return zero;
                }
                else
                {
                    const T args = one+squared(absa/absb);
                    return  absb * sqrt_of(args);
                }
            }
        }

        
        
    }
}

#endif

