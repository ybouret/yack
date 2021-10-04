//! \file

#ifndef YACK_SYNC_QUARK_MUTEX_INCLUDED
#define YACK_SYNC_QUARK_MUTEX_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            struct mutex;
            mutex *mutex_create();                  //!< create a new recursive mutex
            void   mutex_delete(mutex *)   throw(); //!< delete an old recursive mutex
            void   mutex_lock(mutex*)      throw(); //!< lock
            void   mutex_unlock(mutex *)   throw(); //!< unlock
            bool   mutex_try_lock(mutex *) throw(); //!< try lock
        }
    }
}

#endif

