
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_LEGACY_INCLUDED
#define YACK_MEMORY_ALLOCATOR_LEGACY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! legacy interface, base on pooled allocator
        //
        //______________________________________________________________________
        struct legacy
        {
            static void *acquire(const size_t n);          //!< acquire, may throw
            static void *c_alloc(const size_t n) throw();  //!< acquire, no throw
            static void  release(void *) throw();          //!< release
        };

    }

}

#endif

