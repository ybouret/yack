
#include "yack/data/linked.hpp"

namespace yack
{
    
    linked:: ~linked() noexcept
    {
        assert(0==size);
    }
    
    linked:: linked() noexcept : size(0)
    {
    }
    
    void linked:: increase() noexcept
    {
        ++coerce(size);
    }
    
    void linked:: decrease() noexcept
    {
        assert(size>0);
        --coerce(size);
    }
    
    void linked:: xch_size(linked &other) noexcept
    {
        coerce_cswap(size,other.size);
    }
    
}


