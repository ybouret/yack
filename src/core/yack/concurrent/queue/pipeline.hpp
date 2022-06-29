//! \file

#ifndef YACK_CONCURRENT_PIPELINE_INCLUDED
#define YACK_CONCURRENT_PIPELINE_INCLUDED 1

#include "yack/concurrent/queue.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/topology.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {
        class pipeline
        {
        public:
            static const char clid[];

            class worker
            {
            public:
                worker(pipeline    &boss,
                       const size_t size,
                       const size_t rank) ;
                ~worker() throw();

                worker       *next; //!< for lists
                worker       *prev; //!< for lists
                pipeline     &host; //!< origin
                condition     cond; //!< my condition
                void         *task; //!< task to do
                const context ctx;  //!< context
                const thread  thr;  //!< thread

                //! return worker[1..capa], capa <== bytes
                static worker *zalloc( size_t &capa );

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(worker);
            };

            explicit pipeline(const topology &);
            virtual ~pipeline() throw();
            
            mutex        sync;
            condition    gate;
            const size_t threads;
            size_t       zbytes_;
            worker      *squad;
            size_t       ready;

            void        cycle()       throw();
            static void entry(void *) throw();
            void        zkill() throw();
            void        finish(size_t count) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pipeline);
        };
    }
}

#endif

