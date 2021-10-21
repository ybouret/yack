//! \file
#ifndef Y_SYSTEM_PID_INCLUDED
#define Y_SYSTEM_PID_INCLUDED 1

#include "yack/setup.hpp"

#if defined(YACK_BSD)
#    include <unistd.h>
#endif

namespace yack
{
    //! get the process id
    struct process_id
    {
        //! system dependent type
#if defined(YACK_BSD)
        typedef pid_t   type;
#endif

#if defined(YACK_WIN)
        typedef uint32_t type;
#endif

        static type     get() throw(); //!< get the PID
        static uint32_t h32() throw(); //!< get the crc32 of it
    };


}
#endif

