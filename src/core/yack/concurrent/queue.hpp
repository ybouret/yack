
//! \file

#ifndef YACK_SYNC_QUEUE_INCLUDED
#define YACK_SYNC_QUEUE_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/functor.hpp"
#include "yack/container/sequence.hpp"

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
            virtual job_uuid write(const job_type &J) = 0;
            virtual void     flush() throw()          = 0;
            virtual void     prune() throw()          = 0;
            
            template <typename ITERATOR> inline
            void batch(sequence<job_uuid> &uuid,
                       ITERATOR           &iter,
                       size_t              jobs)
            {
                while(jobs-- > 0)
                {
                    uuid << write(*iter);
                    ++iter;
                }
            }

            template <typename SEQUENCE> inline
            void batch(sequence<job_uuid> &uuid,
                       SEQUENCE            jobs)
            {
                batch(uuid,jobs.begin(),jobs.size());
            }


        protected:
            explicit queue() throw(); //!< setup


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(queue);
        };

    }

}

#endif
