//! \file

#ifndef YACK_SYNC_THREAD_INCLUDED
#define YACK_SYNC_THREAD_INCLUDED 1

#include "yack/synchronic/quark/threadable.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            class thread;
        }

        class thread
        {
        public:
            explicit thread(threadable proc, void *args);
            virtual ~thread() throw();
            
        private:
            quark::thread *impl;
        };
    }
}

#endif
