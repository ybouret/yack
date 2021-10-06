
#include "yack/memory/book.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

#include <new>
#include <iostream>

namespace yack
{

    namespace memory
    {

        book:: ~book() throw()
        {
            for(size_t p=max_page_exp2;p>=min_page_exp2;--p)
            {
                destruct( &chapters[p] );
            }
        }

        book:: book() throw() : chapters(0), impl()
        {
            chapter *ch = (chapters = static_cast<chapter *>( out_of_reach::zset(impl,sizeof(impl) ) )-min_page_exp2);
            for(size_t p=min_page_exp2,n=min_page_size;p<=max_page_exp2;++p,n<<=1)
            {
                new (ch+p) chapter(n);
            }

        }


        void *book:: query(const size_t page_exp2)
        {
            assert(page_exp2>=min_page_exp2);
            assert(page_exp2<=max_page_exp2);
            return chapters[page_exp2].query();
        }

        void  book:: store(void *addr, const size_t page_exp2) throw()
        {
            assert(NULL!=addr);
            assert(page_exp2>=min_page_exp2);
            assert(page_exp2<=max_page_exp2);
            chapters[page_exp2].store(addr);
        }



      
    }

}
