//! \file

#ifndef YACK_SYNC_CONDITION_INCLUDED
#define YACK_SYNC_CONDITION_INCLUDED 1

#include "yack/concurrent/primitive.hpp"

namespace yack
{
    namespace concurrent
    {
        class mutex;

        namespace quark
        {
            class condition;
        }
        
        //______________________________________________________________________
        //
        //
        //! condition object
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
            virtual ~condition()   throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     wait(mutex &) throw(); //!< wait on a LOCKED mutex
            void     signal()      throw(); //!< signal one waiting thread
            void     broadcast()   throw(); //!< broadcast all waiting threads

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(condition);
            quark::condition *impl;
        };
    }
}

#endif

