
#include "yack/apex/xreal.hpp"
#include "yack/system/exception.hpp"

#include <cerrno>
#include <cmath>
#include <cfloat>
#include <iostream>

namespace yack
{
    namespace apex
    {
        template <> const float xreal<float>::maximum = FLT_MAX;
        template <> const float xreal<float>::minimum = FLT_MIN;
        template <> const float xreal<float>::epsilon = FLT_EPSILON;

        template <> const double xreal<double>::maximum = DBL_MAX;
        template <> const double xreal<double>::minimum = DBL_MIN;
        template <> const double xreal<double>::epsilon = DBL_EPSILON;

        template <> const long double xreal<long double>::maximum = LDBL_MAX;
        template <> const long double xreal<long double>::minimum = LDBL_MIN;
        template <> const long double xreal<long double>::epsilon = LDBL_EPSILON;

        template <> const int xreal<float>::       max_exp = FLT_MAX_EXP;
        template <> const int xreal<double>::      max_exp = DBL_MAX_EXP;
        template <> const int xreal<long double>:: max_exp = LDBL_MAX_EXP;

        template <> const int xreal<float>::       min_exp = FLT_MIN_EXP;
        template <> const int xreal<double>::      min_exp = DBL_MIN_EXP;
        template <> const int xreal<long double>:: min_exp = LDBL_MIN_EXP;



    }
}

#define real_t float
#include "xreal.hxx"

#undef  real_t
#define real_t double
#include "xreal.hxx"


#undef  real_t
#define real_t long double
#include "xreal.hxx"


