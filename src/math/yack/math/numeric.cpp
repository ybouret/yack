#include "yack/math/numeric.hpp"
#include <cmath>
#include <cfloat>


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

    }

}

