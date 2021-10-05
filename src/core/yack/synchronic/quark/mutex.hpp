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
            struct    mutex_api
            {
                static mutex *create();
                static void   destruct(mutex *) throw();
                static void   lock(mutex *)     throw();
                static void   unlock(mutex *)   throw();
                static bool   try_lock(mutex *) throw();
            };

            
        }
    }
}

#endif

