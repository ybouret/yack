

#include "yack/math/api.hpp"
#include "yack/math/xcomplex.hpp"

namespace yack
{
    namespace math
    {
        // float
        template <>
        extended<float> mod2_of< xcomplex<float> >(const  xcomplex<float> &x)
        {
            return x.mod2();
        }

        template <>
        extended<float> abs_of< xcomplex<float> >(const  xcomplex<float> &x)
        {
            return x.mod2().sqrt();
        }

        // double
        template <>
        extended<double> mod2_of< xcomplex<double> >(const  xcomplex<double> &x)
        {
            return x.mod2();
        }

        template <>
        extended<double> abs_of< xcomplex<double> >(const  xcomplex<double> &x)
        {
            return x.mod2().sqrt();
        }

        // long double
        template <>
        extended<long double> mod2_of< xcomplex<long double> >(const  xcomplex<long double> &x)
        {
            return x.mod2();
        }

        template <>
        extended<long double> abs_of< xcomplex<long double> >(const  xcomplex<long double> &x)
        {
            return x.mod2().sqrt();
        }


    }


}




