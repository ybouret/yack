
//! \file

#ifndef YACK_SYNC_QUEUE_AGENT_INCLUDED
#define YACK_SYNC_QUEUE_AGENT_INCLUDED 1

#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list.hpp"

namespace yack
{
    namespace concurrent
    {
        
        class agent
        {
        public:
            agent(size_t    &,
                  mutex     &,
                  condition &);
            ~agent() throw();

            agent       *next;
            agent       *prev;
            size_t      &live;
            mutex       &sync;
            condition   &comm;
            condition    cond;
            const size_t indx;
            static    void enroll(void *) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agent);
            thread    thr;
            void      mission() throw();

        };


        class agency
        {
        public:
            typedef list_of<agent> manifest;
            explicit agency(size_t n);
            virtual ~agency() throw();



        private:
            manifest          working;
            manifest          waiting;
            mutex             sync;
            condition         comm;
            size_t            live;
            cxx_series<agent> crew;
        };

    }

}

#endif
