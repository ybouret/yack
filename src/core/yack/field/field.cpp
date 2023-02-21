#include "yack/field/field.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{

    field:: ~field() noexcept
    {
        if(wksp)
        {
            static memory::allocator &mgr = memory::dyadic::location();
            assert(wlen>0);
            mgr.release(wksp,wlen);
        }
    }

    const string & field:: key() const noexcept { return name; }

    void  field:: allocate(memory::embed emb[], const size_t num)
    {
        static memory::allocator &mgr = memory::dyadic::instance();
        assert(NULL!=emb);
        assert(num>0);
        wksp = memory::embed::zalloc(emb,num,mgr,wlen);
    }

    size_t field:: granted() const noexcept
    {
        return wlen;
    }
}

