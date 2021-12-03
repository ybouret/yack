#include "yack/math/numeric.hpp"
#include <cmath>
#include <cfloat>
#include "yack/arith/ipower.hpp"

#define YACK_NUMERIC(NAME,VALUE) \
template <> const float       numeric<float>       :: NAME = FLT_##VALUE;\
template <> const double      numeric<double>      :: NAME = DBL_##VALUE;\
template <> const long double numeric<long double> :: NAME = LDBL_##VALUE


namespace yack
{
    namespace math
    {
        YACK_NUMERIC(epsilon,EPSILON);
        YACK_NUMERIC(minimum,MIN);
        YACK_NUMERIC(maximum,MAX);

        template <> const float       numeric<float>::       golden_r = (sqrt(5.0f) - 1.0f)/2;
        template <> const double      numeric<double>::      golden_r = (sqrt(5.0)  - 1.0)/2;
        template <> const long double numeric<long double>:: golden_r = (sqrt(5.0L) - 1.0L)/2;

        template <> const float       numeric<float>::       golden_c = 1.0f - numeric<float>::golden_r;
        template <> const double      numeric<double>::      golden_c = 1.0  - numeric<double>::golden_r;
        template <> const long double numeric<long double>:: golden_c = 1.0L - numeric<long double>::golden_r;

        template <> const float       numeric<float>::       golden_i = 1.0f + numeric<float>::golden_r;
        template <> const double      numeric<double>::      golden_i = 1.0  + numeric<double>::golden_r;
        template <> const long double numeric<long double>:: golden_i = 1.0L + numeric<long double>::golden_r;

        template <> const unsigned numeric<float>      ::dig = FLT_DIG;
        template <> const unsigned numeric<double>     ::dig = DBL_DIG;
        template <> const unsigned numeric<long double>::dig = LDBL_DIG;

        template <> const float       numeric<float>::       ftol = ipower<float,unsigned>(0.1f,dig);
        template <> const double      numeric<double>::      ftol = ipower<double,unsigned>(0.1,dig);
        template <> const long double numeric<long double>:: ftol = ipower<long double,unsigned>(0.1L,dig);

    }

}

