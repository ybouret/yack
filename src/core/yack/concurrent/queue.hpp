
//! \file

#ifndef YACK_SYNC_QUEUE_INCLUDED
#define YACK_SYNC_QUEUE_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/functor.hpp"
#include "yack/container/sequence.hpp"
#include "yack/concurrent/assembly.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        // global types and definitions
        //
        //______________________________________________________________________

        //! job identifier to check status
        typedef uint32_t                     job_uuid;

        //! job type
        /**
         - context  : is passed to access private resources
         - lockable : is passed to access shared resources
         */
        typedef functor<void,TL2(const context &,lockable&)> job_type; //!< alias

     
        //______________________________________________________________________
        //
        //
        //! queue of jobs
        //
        //______________________________________________________________________
        class queue : public assembly
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~queue() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual job_uuid write(const job_type &J)            = 0; //!< write a new job
            virtual void     flush()                     throw() = 0; //!< flush all jobs
            virtual void     prune()                     throw() = 0; //!< prune pending jobs
            virtual bool     ended(const job_uuid) const throw() = 0; //!< check if ended

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________

            //! append a batch of jobs to queue and their IDs to uuid
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

            //! append a batch of jobs to queue and their IDs to uuid
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
