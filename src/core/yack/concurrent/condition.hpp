//! \file

#ifndef YACK_SYNC_CONDITION_INCLUDED
#define YACK_SYNC_CONDITION_INCLUDED 1

#include "yack/concurrent/primitive.hpp"

namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        // forward declarations
        //
        //______________________________________________________________________
        class mutex;
        namespace quark { class condition; }
        
        //______________________________________________________________________
        //
        //
        //! condition variable
        /**
         The system's condition is hold within the quark::condition.
         the methods are then directly implemented withing condition-api.hxx
         */
        //
        //______________________________________________________________________
        class condition : public primitive
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit condition();           //!< create internal condition
            virtual ~condition()   noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     wait(mutex &) noexcept; //!< wait on a LOCKED mutex
            void     signal()      noexcept; //!< signal one waiting thread
            void     broadcast()   noexcept; //!< broadcast all waiting threads

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(condition);
            quark::condition *cond;
        };
    }
}

#endif

