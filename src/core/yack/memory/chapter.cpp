
#include "yack/memory/chapter.hpp"

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

        
    }

}
