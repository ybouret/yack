
#include "yack/counting/mloop.hpp"
#include "yack/memory/allocator/pooled.hpp"


namespace yack
{

    namespace kernel
    {

        mloop:: ~mloop() throw()
        {
            if(wksp)
            {
                memory::allocator &mgr = memory::pooled::instance();
                mgr.release(wksp,wlen);
            }
        }

        mloop:: mloop(const size_t dim) throw() :
        levels(dim),
        frames(0),
        wksp(0),
        wlen(0)
        {
        }

        size_t mloop:: granted() const throw()
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

