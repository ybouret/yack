//! \file

#ifndef YACK_SYNC_THREAD_INCLUDED
#define YACK_SYNC_THREAD_INCLUDED 1

#include "yack/concurrent/quark/threadable.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        // forward declaration
        //
        //______________________________________________________________________
        namespace quark
        {
            class thread;
        }

        //______________________________________________________________________
        //
        //
        //! thread creation and startup
        /**
         The methods are directly implemented in thread-api.hxx
         */
        //
        //______________________________________________________________________
        class thread : public threadable
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit thread(procedure proc, void *args); //!< initialize
            virtual ~thread() throw();                   //!< wait and destruct
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(thread);
            quark::thread *impl;
        };
    }
}

#endif
