//! \file

#ifndef YACK_CONCURRENT_PIPELINE_INCLUDED
#define YACK_CONCURRENT_PIPELINE_INCLUDED 1

#include "yack/concurrent/queue/pipeline/drone.hpp"
#include "yack/concurrent/queue/pipeline/jpool.hpp"
#include "yack/concurrent/topology.hpp"
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
        class pipeline : public queue
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char              clid[]; //!< "concurrent::pipeline"
            typedef randomized::ParkMiller prng;   //!< pseudo-random number generator
            typedef list_of<jnode>         jlist;  //!< list of alive/zombie jobs
            typedef list_of<drone>         drones; //!< available/computing drones
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit pipeline(const topology &); //!< initialize w.r.t topology
            virtual ~pipeline() noexcept;         //!< cleanup

            //__________________________________________________________________
            //
            // assembly interface
            //__________________________________________________________________
            virtual size_t      size()                        const noexcept; //!< threads
            virtual const_type &operator[](const size_t indx) const noexcept; //!< in 1..threads

            //__________________________________________________________________
            //
            // queue interface
            //__________________________________________________________________
            virtual job_uuid write(const job_type &J);
            virtual void     flush() noexcept;
            virtual void     prune() noexcept;
            virtual bool     ended(const job_uuid) const noexcept;
            
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
            jlist        pending;   //!< pending jobs
            jpool        zombies;   //!< memory for jobs
            condition    gate;      //!< gate synchronization
            size_t       bytes;     //!< private bytes
            drone       *squad;     //!< drones
            size_t       ready;     //!< to build

            void        cycle()       noexcept; //!< main cycle method
            static void entry(void *) noexcept; //!< forward to cycle

            void        finish(size_t count)  noexcept; //!< cleanup [0..count]
            job_uuid    process(jnode *alive) noexcept; //!< send to computing or to pending
            void        recycle(drone *me)    noexcept; //!< computing->available at random front/back
            void        zkill() noexcept;

        public:
            prng ran; //!< random generator for thread dispatch

        };
    }
}

#endif

