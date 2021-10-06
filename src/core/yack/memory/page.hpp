

//! \file

#ifndef YACK_MEMORY_PAGE_INCLUDED
#define YACK_MEMORY_PAGE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace memory
    {

        struct page
        {
            page  *next;
            page  *prev;

            //! acquire a new blank page
            /**
             \param size is a power of two >= sizeof(page)
             */
            static page *acquire(const size_t size);
            static void  release(page *, size_t size) throw();
        };


    }

}

#endif
