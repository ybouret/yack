
//! \file

#ifndef YACK_SYNC_QUARK_THREAD_INCLUDED
#define YACK_SYNC_QUARK_THREAD_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{

    namespace synchronic
    {

        namespace quark
        {
            class    thread;
            typedef void (*thread_call)(void *);

            thread *  thread_create(thread_call,void *); //!< create a new  thread
            void      thread_delete(thread *)      throw(); //!< delete an old recursive mutex

        }
    }
}

#endif

