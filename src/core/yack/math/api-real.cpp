#include "yack/math/api.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        // mod2
        template <>
        float mod2_of<float>(const float &x)
        {
            return x*x;
        }
        
        template <>
        double mod2_of<double>(const double &x)
        {
            return x*x;
        }
        
        template <>
        long double mod2_of<long double>(const long double &x)
        {
            return x*x;
        }

        // abs
        template <>
        float abs_of<float>(const float &x)
        {
            return fabsf(x);
        }
        
        template <>
        double abs_of<double>(const double &x)
        {
            return fabs(x);
        }
        
        template <>
        long double abs_of<long double>(const long double &x)
        {
            return fabsl(x);
        }
        
        //sqrt
        template <> float       sqrt_of<float>(const float             &f) { return sqrtf(f); }
        template <> double      sqrt_of<double>(const double           &f) { return sqrt(f);  }
        template <> long double sqrt_of<long double>(const long double &f) { return sqrtl(f); }

    }
    
    
}




