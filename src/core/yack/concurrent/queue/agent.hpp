
//! \file

#ifndef YACK_SYNC_QUEUE_AGENT_INCLUDED
#define YACK_SYNC_QUEUE_AGENT_INCLUDED 1

#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack
{
    namespace concurrent
    {
        
        class agent
        {
        public:
            agent(mutex &, size_t &);
            ~agent() throw();

            agent    *next;
            agent    *prev;
            mutex    &sync;
            condition cond;
            size_t   &ready;
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
            explicit agency(size_t n);
            virtual ~agency() throw();
            
        private:
            cxx_series<agent> crew;
            mutex             sync;
            size_t            ready;
        };

    }

}

#endif
