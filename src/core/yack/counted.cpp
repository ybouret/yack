#include "yack/counted.hpp"

namespace yack
{
    counted:: ~counted() throw()
    {
        assert(0==references);
    }
    
    counted:: counted() throw() : references(0)
    {
        
    }
    
    void counted:: withhold() throw()
    {
        ++coerce(references);
    }
        
    bool counted:: liberate() throw()
    {
        assert(references>0);
        return --coerce(references) <= 0;
    }
    
}
