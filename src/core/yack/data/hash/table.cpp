#include "yack/data/hash/table.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace kernel
    {
        size_t  hash_table_:: check(const size_t usr_exp2, const size_t max_exp2)
        {
            if(usr_exp2>max_exp2)
                throw libc::exception(ENOMEM,"hash_table required too many slots");
            return usr_exp2;
        }

        void * hash_table_:: acquire(const size_t page_exp2)
        {
            static memory::dyadic &mgr = memory::dyadic::instance();
            return mgr.query(page_exp2);
        }

        void hash_table_:: release(void *block_addr, const size_t page_exp2) noexcept
        {
            assert(block_addr);
            static memory::dyadic &mgr = memory::dyadic::location();
            mgr.store(block_addr,page_exp2);
        }


    }
}
