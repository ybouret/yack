
//! \file

#ifndef YACK_MEMORY_OWNERSHIP_INCLUDED
#define YACK_MEMORY_OWNERSHIP_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        enum ownership
        {
            owned_by_prev,
            owned_by_self,
            owned_by_next
        };
    }
}

#endif
