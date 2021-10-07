
#include "yack/memory/book.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"


#include <new>

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

        book:: book() throw()  : chapters(0), impl()
        {

            chapter *ch = (chapters = static_cast<chapter *>( out_of_reach::zset(impl,sizeof(impl) ) )-min_page_exp2);
#if 0
            arena   *ar = (arenas   = static_cast<arena *>(   out_of_reach::zset(small_impl,sizeof(small_impl) ) ) );
            {
                size_t  na  = 0;
                try
                {
                    allocator &dispatcher = global::instance();
                    size_t     page_size  = 1;
                    while(na<small_parts)
                    {
                        new (ar+na) arena(page_size,dispatcher,false);
                        ++na;
                        page_size <<= 1;
                    }
                }
                catch(...)
                {
                    while(na>0)
                    {
                        destruct( &ar[--na] );
                    }
                    throw;
                }
            }
#endif

            for(size_t p=min_page_exp2,n=min_page_size;p<=max_page_exp2;++p,n<<=1)
            {
                new (ch+p) chapter(n);
            }

        }


        void *book:: query(const size_t page_exp2)
        {
            assert(page_exp2<=max_page_exp2);
            assert(page_exp2>=min_page_exp2);
            return  chapters[page_exp2].query();
        }

        void  book:: store(void *addr, const size_t page_exp2) throw()
        {
            assert(NULL!=addr);
            assert(page_exp2<=max_page_exp2);
            assert(page_exp2>=min_page_exp2);
            chapters[page_exp2].store(addr);
        }




    }

}

#include <iostream>
namespace yack
{
    namespace memory
    {
        void  book:: display() throw()
        {
            std::cerr << "  <book>" << std::endl;
            for(size_t i=min_page_exp2;i<=max_page_exp2;++i)
            {
                const chapter &ch = chapters[i];
                if(ch.size) ch.display();
            }
            std::cerr << "  <book/>" << std::endl;
        }
    }

}
