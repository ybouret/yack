//! \file
#ifndef YACK_SYSTEM_HW_INCLUDED
#define YACK_SYSTEM_HW_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //! gather hardware information
    struct hardware
    {
        static size_t nprocs(); //!< number of logical processors
    };

}


#endif
