//! \file

#ifndef YACK_MATH_API_INCLUDED
#define YACK_MATH_API_INCLUDED 1

#include "yack/type/scalar.hpp"

namespace yack
{
    namespace math
    {
        template <typename T>
        typename scalar_for<T>::type mod2( const T & );
    }
}

#endif

