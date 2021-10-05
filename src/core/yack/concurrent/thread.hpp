//! \file

#ifndef YACK_SYNC_THREAD_INCLUDED
#define YACK_SYNC_THREAD_INCLUDED 1

#include "yack/concurrent/quark/threadable.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            class thread;
        }

        //______________________________________________________________________
        //
        //
        //! low level thread creation
        //
        //______________________________________________________________________
        class thread
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit thread(threadable proc, void *args); //!< initialize
            virtual ~thread() throw();                    //!< wait and destruct
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(thread);
            quark::thread *impl;
        };
    }
}

#endif
