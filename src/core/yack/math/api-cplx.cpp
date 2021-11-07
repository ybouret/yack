#include "yack/math/api.hpp"
#include "yack/type/complex.hpp"

namespace yack
{
    namespace math
    {
        template <>
        float mod2_of< complex<float> >( const complex<float> &z )
        {
            return z.mod2();
        }
        
        template <>
        double mod2_of< complex<double> >( const complex<double> &z )
        {
            return z.mod2();
        }
        
        template <>
        long double mod2_of< complex<long double> >( const complex<long double> &z )
        {
            return z.mod2();
        }
        
        
        template <>
        float abs_of< complex<float> >( const complex<float> &z )
        {
            return z.abs();
        }
        
        template <>
        double abs_of< complex<double> >( const complex<double> &z )
        {
            return z.abs();
        }
        
        template <>
        long double abs_of< complex<long double> >( const complex<long double> &z )
        {
            return z.abs();
        }
    }
    
}
