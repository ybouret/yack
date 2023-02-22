//! \file

#ifndef YACK_SYNC_QUARK_MUTEX_INCLUDED
#define YACK_SYNC_QUARK_MUTEX_INCLUDED 1

#include "yack/setup.hpp"

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
                static void   quit(mutex * &)   noexcept;  //!< destruct a mutex
                static void   lock(mutex *)     noexcept;  //!< lock access
                static void   unlock(mutex *)   noexcept;  //!< unlock access
                static bool   try_lock(mutex *) noexcept;  //!< try lock access
            };

            
        }
    }
}

#endif

