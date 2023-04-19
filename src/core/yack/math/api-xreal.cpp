
#include "yack/math/api.hpp"
#include "yack/math/extended.hpp"

namespace yack
{
    namespace math
    {
        // xfloat
        template <>
        extended<float> mod2_of< extended<float> >(const  extended<float> &x)
        {
            return x*x;
        }

        template <>
        extended<float> abs_of< extended<float> >(const  extended<float> &x)
        {
            return x.abs();
        }

        // xdouble
        template <>
        extended<double> mod2_of<extended<double> >(const extended<double> &x)
        {
            return x*x;
        }

        template <>
        extended<double> abs_of<extended<double> >(const extended<double> &x)
        {
            return x.abs();
        }

        // xldbl
        template <>
        extended<long double> mod2_of<extended<long double> >(const extended<long double>  &x)
        {
            return x*x;
        }

        template <>
        extended<long double> abs_of<extended<long double> >(const extended<long double>  &x)
        {
            return x.abs();
        }


    }


}




