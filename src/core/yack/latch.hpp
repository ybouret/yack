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
        explicit     latch()         throw(); //!< off
        virtual     ~latch()         throw(); //!< cleanup
        virtual void lock()          throw(); //!< set to on, or fatal error
        virtual void unlock()        throw(); //!< set to off, or fatal error
        virtual bool try_lock()      throw(); //!< test/set
        bool         latched() const throw(); //!< query status

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(latch);
        bool on;

    };

}

#endif
