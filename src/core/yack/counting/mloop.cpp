
#include "yack/counting/mloop.hpp"
#include "yack/memory/allocator/pooled.hpp"


namespace yack
{

    namespace kernel
    {

        mloop:: ~mloop() noexcept
        {
            if(wksp)
            {
                memory::allocator &mgr = memory::pooled::instance();
                mgr.release(wksp,wlen);
            }
        }

        mloop:: mloop(const size_t dim) noexcept :
        levels(dim),
        wksp(0),
        wlen(0)
        {
        }

        size_t mloop:: granted() const noexcept
        {
            return wlen;
        }

        void mloop:: allocate_cxx(memory::embed emb[], const size_t num)
        {
            static memory::allocator &mgr = memory::pooled::instance();
            assert(emb);
            assert(num);
            assert(0==wksp);
            assert(0==wlen);
            wksp = memory::embed::zalloc(emb,num,mgr,wlen);
            memory::embed::cxx(emb,num);
        }

    }

}

