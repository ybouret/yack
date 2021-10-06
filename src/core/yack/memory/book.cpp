
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
      
    }

}
