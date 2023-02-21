#include "yack/counting/counting.hpp"
#include "yack/exception.hpp"

namespace yack
{
    
    counting:: counting(const cardinality_t num) noexcept :
    index(1),
    total(num)
    {
        assert(num>0);
    }
    
    counting:: ~counting() noexcept
    {
    }

    void counting:: boot() noexcept
    {
        coerce(index)=1;
        on_boot();
    }

    bool counting:: next()
    {
        assert(index<=total);
        try {
            ++coerce(index);
            return on_next();
        }
        catch(...)
        {
            boot();
            throw;
        }
    }
    

}

