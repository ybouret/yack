
//! \file

#ifndef YACK_SYNC_QUEUE_AGENT_INCLUDED
#define YACK_SYNC_QUEUE_AGENT_INCLUDED 1

#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/object.hpp"

namespace yack
{
    namespace concurrent
    {
        
        class agent
        {
        public:
            agent(mutex &);
            ~agent() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agent);
        };

        

    }

}

#endif
