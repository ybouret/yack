#include "yack/color/rgba/from-float.hpp"
#include "yack/color/unit-real.hpp"

namespace yack
{
    namespace color
    {
        
        from_float:: ~from_float() throw() {}
        
        from_float::  from_float() throw() :
        data_to_rgba( sizeof(float) )
        {}
        
        rgba from_float:: operator()(const void *addr) const throw()
        {
            assert(addr);
            const float   f = *static_cast<const float *>(addr);
            const uint8_t u = unit_real<float>::to_byte(f);
            return rgba(u,u,u,0xff);
        }
        
    }
    
}

