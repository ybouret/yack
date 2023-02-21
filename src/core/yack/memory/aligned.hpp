
//! \file

#ifndef YACK_MEMORY_ALIGNED_INCLUDED
#define YACK_MEMORY_ALIGNED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! memory aligment helpers
        //
        //______________________________________________________________________
        struct aligned
        {
            static const size_t largest;                       //!< using base2
            static size_t       package(const size_t) noexcept; //!< using YACK_MEMALIGN
        };
    }
}

#endif

