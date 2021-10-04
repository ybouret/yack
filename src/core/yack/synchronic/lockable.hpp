
//! \file

#ifndef YACK_LOCKABLE_INCLUDED
#define YACK_LOCKABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! interfacer to lockable objects
    //
    //__________________________________________________________________________
    class lockable
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //__________________________________________________________________________
        virtual void lock()   throw() = 0; //!< lock
        virtual void unlock() throw() = 0; //!< unlock

        //______________________________________________________________________
        //
        //! user defined scope locking
        //__________________________________________________________________________
        class scope
        {
        public:
            scope(lockable &) throw(); //!< lock
            ~scope()          throw(); //!< unlock
            
        private:
            lockable &host;
            YACK_DISABLE_COPY_AND_ASSIGN(scope);
        };

        //______________________________________________________________________
        //
        // C++
        //__________________________________________________________________________
        virtual ~lockable() throw(); //!< cleanup

    protected:
        explicit lockable() throw(); //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(lockable);
    };

    //! create the guard name
#define YACK_LOCK__(X,Y) X##Y

    //! instantiace the guard name
#define YACK_LOCK_(HOST,ID) const yack::lockable::scope  YACK_LOCK__(__guard,ID)(HOST)

    //! use a local scoped_lock to lock HOST
#define YACK_LOCK(HOST) YACK_LOCK_(HOST,__LINE__)



}

#endif
