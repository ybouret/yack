#include "yack/math/extended.hpp"
#include "yack/system/exception.hpp"

#include <cerrno>
#include <cmath>
#include <cfloat>
#include <iostream>

namespace yack
{

    template <> const float    extended<float>::maximum  = FLT_MAX;
    template <> const float    extended<float>::minimum  = FLT_MIN;
    template <> const float    extended<float>::epsilon  = FLT_EPSILON;
    template <> const unsigned extended<float>::mant_dig = FLT_MANT_DIG;

    template <> const double   extended<double>::maximum  = DBL_MAX;
    template <> const double   extended<double>::minimum  = DBL_MIN;
    template <> const double   extended<double>::epsilon  = DBL_EPSILON;
    template <> const unsigned extended<double>::mant_dig = DBL_MANT_DIG;

    template <> const long double extended<long double>::maximum  = LDBL_MAX;
    template <> const long double extended<long double>::minimum  = LDBL_MIN;
    template <> const long double extended<long double>::epsilon  = LDBL_EPSILON;
    template <> const unsigned    extended<long double>::mant_dig = LDBL_MANT_DIG;

    template <> const int extended<float>::       max_exp = FLT_MAX_EXP;
    template <> const int extended<double>::      max_exp = DBL_MAX_EXP;
    template <> const int extended<long double>:: max_exp = LDBL_MAX_EXP;

    template <> const int extended<float>::       min_exp = FLT_MIN_EXP;
    template <> const int extended<double>::      min_exp = DBL_MIN_EXP;
    template <> const int extended<long double>:: min_exp = LDBL_MIN_EXP;


}

#define real_t float
#include "extended.hxx"

#undef  real_t
#define real_t double
#include "extended.hxx"


#undef  real_t
#define real_t long double
#include "extended.hxx"


