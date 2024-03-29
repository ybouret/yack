#include "yack/memory/large/page.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{

    namespace memory
    {

        page * page::acquire(const size_t size)
        {
            static allocator &mgr = global::instance();
            assert(size>=sizeof(page));
            assert(is_a_power_of_two(size));

            size_t count = 1;
            page  *entry = static_cast<page *>( mgr.acquire(count,size) );

            assert(size==count);
            return entry;
        }

        void page:: release(page *entry, size_t size) noexcept
        {
            static allocator &mgr = global::location();

            assert(entry);
            assert(size>=sizeof(page));
            assert(is_a_power_of_two(size));
            assert(NULL==entry->next);
            assert(NULL==entry->prev);
            void *block_addr = entry;
            mgr.release(block_addr,size);
        }

    }

}
