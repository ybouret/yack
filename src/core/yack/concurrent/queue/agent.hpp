
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

        //! todo
        class agent : public object
        {
        public:
            agent       *next;
            agent       *prev;
            const size_t uuid;

            virtual ~agent() throw();
            explicit agent(const size_t uniqID,
                           mutex       &access,
                           size_t      &run_up);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agent);
            mutex     &sync;
            size_t    &left;
            condition  cond;
            thread     self;

            static void call(void *) throw();
            void        mission() throw();
        };


        class agency
        {
        public:

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agency);

        };


    }

}

#endif
