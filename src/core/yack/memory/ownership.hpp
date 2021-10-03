
//! \file

#ifndef YACK_MEMORY_OWNERSHIP_INCLUDED
#define YACK_MEMORY_OWNERSHIP_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    namespace memory
    {
        //______________________________________________________________________
        //
        //! helper to locate memory
        //______________________________________________________________________
        enum ownership
        {
            owned_by_prev, //!< before
            owned_by_self, //!< there!
            owned_by_next  //!< after
        };
    }
}

#endif
