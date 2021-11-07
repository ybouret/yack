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
        
        template <>
        float abs_of<float>(const float &x)
        {
            return fabs(x);
        }
        
        template <>
        double abs_of<double>(const double &x)
        {
            return fabs(x);
        }
        
        template <>
        long double abs_of<long double>(const long double &x)
        {
            return fabs(x);
        }
        
        
    }
    
    
}




