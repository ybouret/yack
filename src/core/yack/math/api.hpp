//! \file

#ifndef YACK_MATH_API_INCLUDED
#define YACK_MATH_API_INCLUDED 1

#include "yack/type/scalar.hpp"

namespace yack
{
    namespace math
    {

        template <typename T> typename scalar_for<T>::type mod2_of(const T &); //!< |x|^2
        template <typename T> typename scalar_for<T>::type abs_of(const T &);  //!< |x|
        template <typename T> T                            sqrt_of(const T &); //!< sqrt(x)

    }
}

#endif

