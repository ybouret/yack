

//! \file

#ifndef YACK_FAKE_LOCK_INCLUDED
#define YACK_FAKE_LOCK_INCLUDED 1

#include "yack/synchronic/lockable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! fake lock for sequential code/testing
    //
    //__________________________________________________________________________
    class fake_lock : public lockable
    {
    public:
        explicit fake_lock() throw(); //!< setup
        virtual ~fake_lock() throw(); //!< cleanup

        void lock()   throw(); //!< increase depth
        void unlock() throw(); //!< decrease depth, error if negative

        const int depth;       //!< locking depth
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(fake_lock);
    };

}

#endif
