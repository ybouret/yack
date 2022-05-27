
//! \file

#ifndef YACK_SYNC_QUEUE_AGENT_INCLUDED
#define YACK_SYNC_QUEUE_AGENT_INCLUDED 1

#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/topology.hpp"
#include "yack/concurrent/queue.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list.hpp"

namespace yack
{
    namespace concurrent
    {


        //! todo
        class agent
        {
        public:
            agent(); //!< todo

            ~agent() throw(); //!< todo

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agent);
        };

    }

}

#endif
