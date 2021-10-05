//! \file

#ifndef YACK_SYNC_QUARK_MUTEX_INCLUDED
#define YACK_SYNC_QUARK_MUTEX_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    class lockable;

    namespace concurrent
    {
        
        namespace quark
        {
            //__________________________________________________________________
            //
            // structure declaration
            //__________________________________________________________________
            struct    mutex;

            //__________________________________________________________________
            //
            //
            //! mutex API
            /**
             the memory of each mutex is within a compact memory::arena
             managed by the internal atelier.
             */
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

