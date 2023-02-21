#include "yack/counting/schedule.hpp"
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/pooled.hpp"

namespace yack
{
    
    schedule:: schedule(const size_t        ctx_size,
                        const size_t        max_size,
                        const cardinality_t num) :
    counting(num),
    addr(0),
    data(0),
    used(0)
    {
        static memory::allocator &mem = memory::pooled::instance();
        uint8_t      *wksp = 0;
        memory::embed emb[] =
        {
            memory::embed(wksp,ctx_size),
            memory::embed(data,max_size)
        };
        addr = YACK_MEMORY_EMBED(emb,mem,used);
        --data;
    }
    
    size_t schedule:: granted() const noexcept
    {
        return used;
    }
    
    schedule:: ~schedule() noexcept
    {
        static memory::allocator &mem = memory::pooled::location();
        mem.release(addr,used);
        data = 0;
    }
    
    size_t & schedule:: operator[](const size_t indx) noexcept
    {
        assert(indx>=1);
        assert(indx<=size());
        return data[indx];
    }
    
    const size_t & schedule:: operator[](const size_t indx) const noexcept
    {
        assert(indx>=1);
        assert(indx<=size());
        return data[indx];
    }

    
}

