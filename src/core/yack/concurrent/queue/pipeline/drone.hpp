
//! \file

#ifndef YACK_CONCURRENT_PIPELINE_DRONE_INCLUDED
#define YACK_CONCURRENT_PIPELINE_DRONE_INCLUDED 1

#include "yack/concurrent/queue/pipeline/jnode.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/concurrent/mutex.hpp"


namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //! threaded drone
        //______________________________________________________________________
        class drone
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            drone(const size_t size,
                  const size_t rank,
                  quark::threadable::procedure proc,
                  void                        *args); //!< setup
            ~drone() throw();                         //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            drone        *next; //!< for lists
            drone        *prev; //!< for lists
            condition     cond; //!< self-condition
            jNode        *task; //!< task to do
            const context ctx;  //!< context
            const thread  thr;  //!< thread

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! return drone[1..capa], capa <== bytes
            static drone *zalloc( size_t &capa );

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(drone);
        };


    }

}

#endif
