//! \file

#ifndef YACK_LATCH_INCLUDED
#define YACK_LATCH_INCLUDED 1

#include "yack/lockable.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! implements a lockable latch
    //
    //__________________________________________________________________________
    class latch : public lockable
    {
    public:
        explicit     latch()         noexcept; //!< off
        virtual     ~latch()         noexcept; //!< cleanup
        virtual void lock()          noexcept; //!< set to on, or fatal error
        virtual void unlock()        noexcept; //!< set to off, or fatal error
        virtual bool try_lock()      noexcept; //!< test/set
        bool         latched() const noexcept; //!< query status

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(latch);
        bool on;

    };

}

#endif
