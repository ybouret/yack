#include "yack/gfx/bitrows.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace graphic
    {

        bitrows:: ~bitrows() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(row,mem);
        }

        static inline
        bitrow *bitrows_acquire(size_t &mem)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            return static_cast<bitrow *>(mgr.acquire(mem,1));
        }

        bitrows:: bitrows(const metrics &m, void *p) :
        zfw(m.w),
        zfh(m.h),
        mem(m.h*sizeof(bitrow)),
        row( bitrows_acquire(mem) )
        {
            uint8_t     *q = static_cast<uint8_t *>(p);
            const unit_t h = m.h;
            const unit_t s = m.s;
            for(unit_t y=0;y<h;++y,q+=s)
            {
                new (row+y) bitrow(q,zfw);
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


