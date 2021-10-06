
#include "yack/memory/chapter.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    namespace memory
    {
        chapter:: chapter(const size_t sz) throw() :
        page_size(sz)
        {
            assert(page_size>=sizeof(page));
        }

        chapter:: ~chapter() throw()
        {
            while(size) page::release(pop_back(),page_size);
        }

        page  * chapter:: query()
        {
            if(size)
            {
                page *p = pop_front();
                out_of_reach::zset(p,page_size);
                return p;
            }
            else
            {
                return page::acquire(page_size);
            }
        }

    }

}
