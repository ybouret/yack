
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

        void  * chapter:: query()
        {
            if(size)
            {
                page *p = pop_front();
                return out_of_reach::zset(p,page_size);
            }
            else
            {
                return  page::acquire(page_size);
            }
        }

        void chapter:: store(void *addr) throw()
        {
            // get page with clean header
            assert(addr);
            page *p =  push_back(static_cast<page*>(out_of_reach::zset(addr,sizeof(page))));

            // order by increasing address
            while(p->prev&&(p->prev>p))
            {
                p = towards_front(p);
            }
        }

    }

}
