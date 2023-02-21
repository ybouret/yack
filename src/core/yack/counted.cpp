#include "yack/counted.hpp"

namespace yack
{
    counted:: ~counted() noexcept
    {
        assert(0==nref);
    }
    
    counted:: counted() noexcept : nref(0)
    {
        
    }
    
    void counted:: withhold() noexcept
    {
        ++nref;
    }
        
    bool counted:: liberate() noexcept
    {
        assert(nref>0);
        return --nref <= 0;
    }

    size_t counted:: quantity() const noexcept
    {
        return nref;
    }

}
