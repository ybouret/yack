
//! \file

#ifndef YACK_SYNC_MUTEX_INCLUDED
#define YACK_SYNC_MUTEX_INCLUDED 1

#include "yack/lockable.hpp"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            struct mutex;
        }
        
        //______________________________________________________________________
        //
        //
        //! recursive mutex
        //
        //______________________________________________________________________
        class mutex : public lockable
        {
        public:
            explicit mutex();          //!< create internal mutex
            virtual ~mutex() throw();  //!< cleanup

            virtual void lock()     throw(); //!< lock
            virtual void unlock()   throw(); //!< unlock
            virtual bool try_lock() throw(); //!< try lock
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mutex);
            quark::mutex *impl;
        };
    }
}

#endif

