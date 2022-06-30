//! \file

#ifndef YACK_CONCURRENT_PIPELINE_INCLUDED
#define YACK_CONCURRENT_PIPELINE_INCLUDED 1

#include "yack/concurrent/queue/jpool.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/topology.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/concurrent/mutex.hpp"
#include "yack/data/list/raw.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/data/pool.hpp"


namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //! list of alive jNode
        //______________________________________________________________________
        typedef list_of<jNode> jList;


        //______________________________________________________________________
        //
        //
        //! multi-threaded queue
        //
        //______________________________________________________________________
        class pipeline : public queue
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
                drone        *next; //!< for lists
                drone        *prev; //!< for lists
                condition     cond; //!< self-condition
                jNode        *task; //!< task to do
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

            //__________________________________________________________________
            //
            // queue interface
            //__________________________________________________________________
            virtual job_uuid write(const job_type &J);
            virtual void     flush() throw();
            virtual void     prune() throw();
            virtual bool     ended(const job_uuid) const throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! wrapper to process host/method functor
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            job_uuid operator()(OBJECT_POINTER o, METHOD_POINTER m)
            {
                return process( zombies.build(o,m) );
            }

            //! wrapper to process functionoid
            template <typename FUNCTION>
            job_uuid operator()(FUNCTION &func)
            {
                return process( zombies.build(func) );
            }



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            mutex        sync;      //!< shared mutex
            const size_t threads;   //!< number of threads

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pipeline);
            drones       available; //!< available drones
            drones       computing; //!< computing drones
            jList        pending;   //!< pending jobs
            jPool        zombies;   //!< memory for jobs
            condition    gate;      //!< gate synchronization
            size_t       bytes;     //!< private bytes
            drone       *squad;     //!< drones
            size_t       ready;     //!< to build

            void        cycle()       throw(); //!< main cycle method
            static void entry(void *) throw(); //!< forward to cycle

            void        finish(size_t count)  throw(); //!< cleanup [0..count]
            job_uuid    process(jNode *alive) throw(); //!< send to computing or to pending
            void        recycle(drone *me)    throw(); //!< computing->available at random front/back
            void        zkill() throw();

        public:
            prng ran; //!< random generator for thread dispatch

        };
    }
}

#endif

