#include "yack/gfx/types.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include <cstring>






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
    }

}

namespace yack
{
    namespace graphic
    {
        namespace nexus
        {
            bitrow:: ~bitrow() throw()
            {}

            bitrow:: bitrow(void *entry, const unit_t width, const zero_flux &zflux) throw() :
            p(entry),
            w(width),
            z(zflux)
            {
            }

        }
    }
}

namespace yack
{
    namespace graphic
    {
        namespace nexus
        {
            bitrows:: ~bitrows() throw()
            {
                bitmem:: release(*(void **)&row,mem);
            }

            bitrows:: bitrows(const metrics &m, void *p) :
            zfw(m.w),
            zfh(m.h),
            mem(m.h*sizeof(bitrow)),
            row( static_cast<bitrow*>(bitmem::acquire(mem)) )
            {
                uint8_t     *q = static_cast<uint8_t *>(p);
                const unit_t h = m.h;
                const unit_t w = m.w;
                const unit_t s = m.s;
                for(unit_t y=0;y<h;++y,q+=s)
                {
                    new (row+y) bitrow(q,w,zfw);
                }
            }

            bitrow & bitrows:: operator()(const unit_t y) throw()
            {
                assert(y>=0); assert(y<zfh.size);
                return row[y];
            }

            const bitrow & bitrows:: operator()(const unit_t y) const throw()
            {
                assert(y>=0); assert(y<zfh.size);
                return row[y];
            }

            bitrow & bitrows:: operator[](const unit_t y) throw()
            {
                return row[ zfh(y) ];
            }

            const bitrow & bitrows:: operator[](const unit_t y) const throw()
            {
                return row[ zfh(y) ];
            }

        }

    }
}





namespace yack
{
    namespace graphic
    {
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


