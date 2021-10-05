
//! \file

#ifndef YACK_SYNC_QUARK_CONDITION_INCLUDED
#define YACK_SYNC_QUARK_CONDITION_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            
            struct mutex;
            class  condition;

            struct condition_api
            {
                static condition *create();                          //!< create a condition
                static void       destruct(condition *)     throw(); //!< delete a condition
                static void       wait(condition *,mutex *) throw(); //!< wait on a LOCKED mutex, wakeup on a LOCKED mutex
                static void       signal(condition *)       throw(); //!< signal one waiting thread
                static void       broadcast(condition *)    throw(); //!< broadcast all waiting threads
            };
        }
    }
}

#endif

