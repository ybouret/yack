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
        //______________________________________________________________________
        //
        //
        //! multi-threaded queue
        //
        //______________________________________________________________________
        class pipeline
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< "concurrent::pipeline"

            //__________________________________________________________________
            //
            //! threaded drone
            //__________________________________________________________________
            class drone
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                drone(pipeline &boss, const size_t size, const size_t rank); //!< setup
                ~drone() throw();                                            //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                drone       *next; //!< for lists
                drone       *prev; //!< for lists
                condition     cond; //!< self-condition
                void         *task; //!< task to do
                const context ctx;  //!< context
                const thread  thr;  //!< thread

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________

                //! return drone[1..capa], capa <== bytes
                static drone *zalloc( size_t &capa );

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(drone);
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit pipeline(const topology &); //!< initialize w.r.t topology
            virtual ~pipeline() throw();         //!< cleanup
            
            mutex        sync;      //!< shared mutex
            const size_t threads;   //!< number of threads
            condition    gate;      //!< gate synchronization


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pipeline);

            size_t       zbytes_; //!< private bytes
            drone       *squad;   //!< drones
            size_t       ready;   //!< to build

            void        cycle()       throw();
            static void entry(void *) throw();
            void        zkill() throw();
            void        finish(size_t count) throw();

        };
    }
}

#endif

