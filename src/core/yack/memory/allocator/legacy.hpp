
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_LEGACY_INCLUDED
#define YACK_MEMORY_ALLOCATOR_LEGACY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {

        struct legacy
        {
            static void *acquire(const size_t n);
            static void *c_alloc(const size_t n) throw();
            static void  release(void *) throw();
        };

    }

}

#endif

