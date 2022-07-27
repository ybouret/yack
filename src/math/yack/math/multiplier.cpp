
#include "yack/math/multiplier.hpp"
#include "yack/math/numeric.hpp"

namespace yack
{
    namespace math
    {
        multiplication::  multiplication() throw() {}
        multiplication:: ~multiplication() throw() {}

        template <> const int multiplier<float>::       min_exp = numeric<float>::min_exp;
        template <> const int multiplier<double>::      min_exp = numeric<double>::min_exp;
        template <> const int multiplier<long double>:: min_exp = numeric<long double>::min_exp;

        template <> const int multiplier<float>::       max_exp = numeric<float>::max_exp;
        template <> const int multiplier<double>::      max_exp = numeric<double>::max_exp;
        template <> const int multiplier<long double>:: max_exp = numeric<long double>::max_exp;



    }

}

