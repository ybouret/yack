

//! \file

#ifndef YACK_MEMORY_PAGE_INCLUDED
#define YACK_MEMORY_PAGE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! blank page, header for power of two bytes pages
        //
        //______________________________________________________________________
        struct page
        {
            page  *next; //!< for list
            page  *prev; //!< for list

            //! acquire a new blank page
            /**
             \param size is a power of two >= sizeof(page)
             */
            static page *acquire(const size_t size);

            //! release an old page
            static void  release(page *, size_t size) throw();
        };


    }

}

#endif
