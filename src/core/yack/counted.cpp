#include "yack/counted.hpp"

namespace yack
{
    counted:: ~counted() throw()
    {
        assert(0==nref);
    }
    
    counted:: counted() throw() : nref(0)
    {
        
    }
    
    void counted:: withhold() throw()
    {
        ++nref;
    }
        
    bool counted:: liberate() throw()
    {
        assert(nref>0);
        return --nref <= 0;
    }

    size_t counted:: quantity() const throw()
    {
        return nref;
    }

}
