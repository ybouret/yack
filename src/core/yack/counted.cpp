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

    YACK_COUNTED_IMPL(counted,nref)

    
}
