//! \file

#ifndef YACK_SYNC_QUARK_THREAD_INCLUDED
#define YACK_SYNC_QUARK_THREAD_INCLUDED 1

#include "yack/concurrent/quark/threadable.hpp"

namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            //__________________________________________________________________
            //
            //
            // thread declaration
            //
            //__________________________________________________________________
            class    thread;
            
            //__________________________________________________________________
            //
            //
            //! thread API
            /**
             Each thread is created from  a compact memory::arena within
             the internal atelier.
            */
            //
            //__________________________________________________________________
            struct   thread_api
            {
                static  thread *  init(threadable &);               //!< create a new  thread
                static  void      quit(thread *   &) noexcept;       //!< finish and delete an old thread
            };

        }
    }
}

#endif

