#include "yack/gfx/bitrows.hpp"
#include "yack/gfx/memory.hpp"

namespace yack
{
    namespace graphic
    {

        bitrows:: ~bitrows() noexcept
        {
            static memory::allocator &mgr = memory_allocator::location();
            mgr.withdraw(row,coerce(mem));
        }

        static inline
        bitrow *bitrows_acquire(size_t &mem)
        {
            static memory::allocator &mgr =  memory_allocator::instance();
            return static_cast<bitrow *>(mgr.acquire(mem,1));
        }

        bitrows:: bitrows(const metrics &m, void *p) :
        zfw(m.w),
        zfh(m.h),
        mem(m.h*sizeof(bitrow)),
        row( bitrows_acquire( coerce(mem) ) )
        {
            uint8_t     *q = static_cast<uint8_t *>(p);
            const unit_t h = m.h;
            const unit_t s = m.s;
            for(unit_t y=0;y<h;++y,q+=s)
            {
                new (row+y) bitrow(q,zfw);
            }
        }

#if 0
        bitrow & bitrows:: operator()(const unit_t y) noexcept
        {
            assert(y>=0); assert(y<zfh.size);
            return row[y];
        }

        const bitrow & bitrows:: operator()(const unit_t y) const noexcept
        {
            assert(y>=0); assert(y<zfh.size);
            return row[y];
        }

        bitrow & bitrows:: operator[](const unit_t y) noexcept
        {
            return row[ zfh(y) ];
        }

        const bitrow & bitrows:: operator[](const unit_t y) const noexcept
        {
            return row[ zfh(y) ];
        }
#endif

    }
}


