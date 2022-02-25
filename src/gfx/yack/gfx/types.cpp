#include "yack/gfx/types.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include <cstring>

namespace yack
{
    namespace graphic
    {
        zero_flux:: zero_flux(unit_t n) throw() :
        size(n),
        szsz(n+n)
        {
        }

        zero_flux:: ~zero_flux() throw()
        {
        }

        unit_t zero_flux:: operator()(unit_t indx) const throw()
        {
            if(indx<0)
            {
                return (*this)(-indx);
            }
            else
            {
                if(indx>=size)
                {
                    return (*this)( szsz - (++indx) );
                }
                else
                {
                    return indx;
                }
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

        }

    }
}


namespace yack
{
    namespace graphic
    {

        namespace nexus
        {
            bitfield:: ~bitfield() throw()
            {
                if(memio)
                {
                    destruct(memio);
                    memio = 0;
                    memset(wksp,0,sizeof(wksp));
                }
                bitmem::release(entry,bytes);
            }

            bitfield:: bitfield(const size_t n) :
            bytes( n ),
            entry( bitmem::acquire(bytes) ),
            memio(NULL),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));
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


