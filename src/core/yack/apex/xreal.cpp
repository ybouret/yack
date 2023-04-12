
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
        
        template <> const double xreal<double>::maximum = DBL_MAX;
        template <> const double xreal<double>::minimum = DBL_MIN;

        template <> const long double xreal<long double>::maximum = LDBL_MAX;
        template <> const long double xreal<long double>::minimum = LDBL_MIN;


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


