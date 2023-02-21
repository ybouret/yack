

//! \file

#ifndef YACK_FAKE_LOCK_INCLUDED
#define YACK_FAKE_LOCK_INCLUDED 1

#include "yack/lockable.hpp"

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
        explicit fake_lock() noexcept; //!< setup
        virtual ~fake_lock() noexcept; //!< cleanup

        void lock()     noexcept; //!< increase depth
        void unlock()   noexcept; //!< decrease depth, error if negative
        bool try_lock() noexcept; //!< lock, true
        const int depth;         //!< locking depth

        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(fake_lock);
    };

}

#endif
