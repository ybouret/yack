#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {
        template <>
        float mod2<float>(const float &x)
        {
            return x*x;
        }
        
        template <>
        double mod2<double>(const double &x)
        {
            return x*x;
        }
        
        template <>
        long double mod2<long double>(const long double &x)
        {
            return x*x;
        }
        
        
    }
    
    
}

#include "yack/type/complex.hpp"

namespace yack
{
    namespace math
    {
        template <>
        float mod2< complex<float> >( const complex<float> &z )
        {
            return z.mod2();
        }
        
        template <>
        double mod2< complex<double> >( const complex<double> &z )
        {
            return z.mod2();
        }
        
        template <>
        long double mod2< complex<long double> >( const complex<long double> &z )
        {
            return z.mod2();
        }
        
    }
    
}

#include "yack/apex.hpp"
namespace yack
{
    namespace math
    {
        template <>
        apn mod2<apn>(const apn &n )
        {
            return n.mod2();
        }
        
        template <>
        apz mod2<apz>(const apz &z )
        {
            return z.mod2();
        }
        
        template <>
        apq mod2<apq>(const apq &q )
        {
            return q.mod2();
        }
        
    }
    
}

