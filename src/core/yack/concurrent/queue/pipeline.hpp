//! \file

#ifndef YACK_CONCURRENT_PIPELINE_INCLUDED
#define YACK_CONCURRENT_PIPELINE_INCLUDED 1

#include "yack/concurrent/queue.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/topology.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/concurrent/mutex.hpp"
#include "yack/data/list/raw.hpp"
#include "yack/randomized/park-miller.hpp"

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
            static const char              clid[]; //!< "concurrent::pipeline"
            typedef randomized::ParkMiller prng;   //!< pseudo-random number generator

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
            //! list of drones
            //__________________________________________________________________
            typedef raw_list_of<drone> drones;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit pipeline(const topology &); //!< initialize w.r.t topology
            virtual ~pipeline() throw();         //!< cleanup
            
            mutex        sync;      //!< shared mutex
            const size_t threads;   //!< number of threads
            drones       available; //!< waiting drone
            drones       computing; //!< working drones

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pipeline);
            condition    gate;    //!< gate synchronization
            size_t       bytes;   //!< private bytes
            drone       *squad;   //!< drones
            size_t       ready;   //!< to build

            void        cycle()       throw();
            static void entry(void *) throw();
            void        zkill() throw();
            void        finish(size_t count) throw();

        public:
            prng ran; //!< random generator for thread dispatch

        };
    }
}

#endif

