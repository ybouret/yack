
//! \file

#ifndef YACK_MATH_SGN_INCLUDED
#define YACK_MATH_SGN_INCLUDED 1

#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {

        //! return the signed value of a w.r.t the sign of b
        template <typename T, typename U> inline
        T sgn(T a, U b)
        {
            return (b >= 0) ? abs_of(a) : -abs_of(a);
        }



    }
}

#endif

