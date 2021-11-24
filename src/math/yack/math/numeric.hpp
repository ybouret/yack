//! \file

#ifndef YACK_MATH_NUMERIC_INCLUDED
#define YACK_MATH_NUMERIC_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace math
    {
        template <typename T>
        struct numeric
        {
            static const T eps;
        };

#define YACK_MATH_NUMERIC(VALUE) \
template <> const float       numeric<float>      ::VALUE; \
template <> const double      numeric<double>     ::VALUE; \
template <> const long double numeric<long double>::VALUE

        YACK_MATH_NUMERIC(eps);

    }

}

#endif
