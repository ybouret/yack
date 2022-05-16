
#include "yack/color/rgba/from-byte.hpp"

namespace yack
{
    namespace color
    {
        
        from_byte:: ~from_byte() throw() {}
        
        from_byte::  from_byte() throw() :
        data_to_rgba( sizeof(uint8_t) )
        {}
        
        rgba from_byte:: operator()(const void *addr) const throw()
        {
            assert(addr);
            const uint8_t u = *static_cast<const uint8_t *>(addr);
            return rgba(u,u,u,0xff);
        }
        
    }
    
}

