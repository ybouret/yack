
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
            struct   thread_api
            {
                typedef void    (*call)(void *);            //!< threadable function
                static  thread *  init(call,void *);        //!< create a new  thread
                static  void      quit(thread * &) throw(); //!< finish and delete an old thread
            };

        }
    }
}

#endif

