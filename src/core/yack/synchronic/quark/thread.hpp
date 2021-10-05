//! \file

#ifndef YACK_SYNC_QUARK_THREAD_INCLUDED
#define YACK_SYNC_QUARK_THREAD_INCLUDED 1

#include "yack/synchronic/quark/threadable.hpp"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            class    thread;
            struct   thread_api
            {
                static  thread *  init(threadable, void *); //!< create a new  thread
                static  void      quit(thread * &) throw(); //!< finish and delete an old thread
            };

        }
    }
}

#endif

