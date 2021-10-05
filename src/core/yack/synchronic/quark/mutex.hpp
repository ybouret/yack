//! \file

#ifndef YACK_SYNC_QUARK_MUTEX_INCLUDED
#define YACK_SYNC_QUARK_MUTEX_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    class lockable;

    namespace synchronic
    {
        
        namespace quark
        {
            struct    mutex;
            //__________________________________________________________________
            //
            //
            //! mutex API
            //
            //__________________________________________________________________
            struct    mutex_api
            {
                static mutex *init();                     //!< initialize a recursive mutex
                static void   quit(mutex * &)   throw();  //!< destruct a mutex
                static void   lock(mutex *)     throw();  //!< lock access
                static void   unlock(mutex *)   throw();  //!< unlock access
                static bool   try_lock(mutex *) throw();  //!< try lock access
            };

            
        }
    }
}

#endif

