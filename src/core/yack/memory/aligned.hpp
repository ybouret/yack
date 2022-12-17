
//! \file

#ifndef YACK_MEMORY_ALIGNED_INCLUDED
#define YACK_MEMORY_ALIGNED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {
        struct aligned
        {
            static const size_t largest;
            static size_t       package(const size_t) throw();
        };
    }
}

#endif

