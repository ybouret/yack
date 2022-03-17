
//! \file

#ifndef YACK_SYNC_QUEUE_AGENT_INCLUDED
#define YACK_SYNC_QUEUE_AGENT_INCLUDED 1

#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/queue.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list.hpp"

namespace yack
{
    namespace concurrent
    {

        class job_node
        {
        public:
            ~job_node() throw();
            job_node(const job_uuid, const job_type &);

            const job_uuid uuid;
            job_node      *next;
            job_node      *prev;
            job_type       todo;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(job_node);

        };

        typedef list_of<job_node> job_list;
        typedef pool_of<job_node> job_pool;



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
            job_list          pending;
            job_list          zombies;
            mutex             sync;
            condition         comm;
            size_t            live;
            cxx_series<agent> crew;
        };

    }

}

#endif
