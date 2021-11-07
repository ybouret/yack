#include "yack/math/api.hpp"
#include "yack/apex.hpp"
namespace yack
{
    namespace math
    {
        template <>
        apn mod2_of<apn>(const apn &n )
        {
            return n.mod2();
        }
        
        template <>
        apz mod2_of<apz>(const apz &z )
        {
            return z.mod2();
        }
        
        template <>
        apq mod2_of<apq>(const apq &q )
        {
            return q.mod2();
        }
        
        
        template <>
        apn abs_of<apn>(const apn &n )
        {
            return n.abs();
        }
        
        template <>
        apz abs_of<apz>(const apz &z )
        {
            return z.abs();
        }
        
        template <>
        apq abs_of<apq>(const apq &q )
        {
            return q.abs();
        }
        
    }
    
}
