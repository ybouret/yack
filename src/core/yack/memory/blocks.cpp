#include "yack/memory/blocks.hpp"
#include "yack/memory/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/memory/allocator/pages.hpp"
#include "yack/arith/base2.hpp"
#include <cerrno>

namespace yack
{

    namespace memory
    {

        blocks:: ~blocks() throw()
        {
            static pages &mgr = pages::location();
            while(size>0)
            {
                destruct( &arenas[--size] );
            }
            mgr.store(arenas,page_exp2);
            arenas    = 0;
            capacity  = 0;
            page_size = 0;
            page_exp2 = 0;
        }

        const char blocks:: designation[] = "memory::blocks";

        static inline
        size_t page_size_for(const size_t bytes)
        {
            if(bytes<=pages::min_page_size)
            {
                return pages::min_page_size;
            }
            else
            {
                if(bytes>pages::max_page_size)
                {
                    throw libc::exception(ENOMEM,"%s exceeds maximal capacity", blocks::designation);
                }
                return next_power_of_two(bytes);
            }
        }

        blocks:: blocks() :
        arenas(0),
        acquiring(0),
        releasing(0),
        size(0),
        capacity(minimal_capacity),
        page_size( page_size_for(capacity*sizeof(arena)) ),
        page_exp2( base2<size_t>::log2_of(page_size)     )
        {
            static pages &mgr = pages::instance();
            assert(is_a_power_of_two(page_size));
            assert((size_t(1) << page_exp2) == page_size);

            arenas = static_cast<arena *>( mgr.query(page_exp2) );
            
        }

        void &blocks:: acquire(const size_t block_size)
        {

        }

    }

}
