#include "yack/gfx/types.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace graphic
    {

        namespace nexus
        {
            void * bitmem:: acquire(size_t &block_size)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                return mgr.acquire(block_size,1);
            }

            void bitmem:: release(void *&block_addr, size_t &block_size) throw()
            {
                static memory::allocator &mgr = memory::dyadic::location();
                mgr.release(block_addr,block_size);
            }
        }



        metrics:: ~metrics() throw()
        {
            coerce(w) = 0;
            coerce(h) = 0;
            coerce(n) = 0;
            coerce(d) = 0;
            coerce(s) = 0;

        }

        metrics:: metrics(const metrics &other) throw() :
        w(other.w),
        h(other.h),
        n(other.n),
        d(other.d),
        s(other.s)
        {
        }

        metrics:: metrics(const unit_t W, const unit_t H, const unit_t D) throw() :
        w(W),
        h(H),
        n(w*h),
        d(D),
        s(w*d)
        {
            assert(w>0);
            assert(h>0);
            assert(d>0);
        }

    }



}


