
//! \file

#ifndef YACK_SYNC_QUEUE_INCLUDED
#define YACK_SYNC_QUEUE_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace concurrent
    {

        typedef uint32_t                     job_uuid; //!< alias
        typedef functor<void,TL1(lockable&)> job_type; //!< alias

     

        //! queue of jobs
        class queue
        {
        public:
            virtual ~queue() throw(); //!< cleanup
            
        protected:
            explicit queue() throw(); //!< setup


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(queue);
        };

    }

}

#endif
