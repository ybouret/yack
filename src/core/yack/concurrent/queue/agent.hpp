
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

        //! job holder
        class job_node
        {
        public:
            ~job_node() throw(); //!< cleanup
            job_node(const job_uuid, const job_type &); //!< setup

            const job_uuid uuid; //!< uuid
            job_node      *next; //!< for list
            job_node      *prev; //!< for list
            job_type       todo; //!< todo

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(job_node);

        };

        typedef list_of<job_node> job_list; //!< alias
        typedef pool_of<job_node> job_pool; //!< alis


        //! agent
        class agent
        {
        public:
            //! setup
            agent(size_t    &,
                  mutex     &,
                  condition &);

            //! cleanup
            ~agent() throw();

            agent       *next; //!< for list
            agent       *prev; //!< for list
            size_t      &live; //!< external counter
            mutex       &sync; //!< external mutex
            condition   &comm; //!< external communication
            condition    cond; //!< private condition
            const size_t indx; //!< uuid

            static    void enroll(void *) throw(); //!< thread entry

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agent);
            thread    thr;
            void      mission() throw();

        };

        //! agency
        class agency
        {
        public:
            typedef list_of<agent> manifest; //!< alias
            explicit agency(size_t n); //!< setup
            virtual ~agency() throw(); //!< cleanup

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
