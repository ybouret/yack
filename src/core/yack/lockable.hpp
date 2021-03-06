
//! \file

#ifndef YACK_LOCKABLE_INCLUDED
#define YACK_LOCKABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! interface to lockable objects
    //
    //__________________________________________________________________________
    class lockable
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual void lock()     throw() = 0; //!< lock
        virtual void unlock()   throw() = 0; //!< unlock
        virtual bool try_lock() throw() = 0; //!< try lock
        
        //______________________________________________________________________
        //
        //! user defined scope locking
        //______________________________________________________________________
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
        //______________________________________________________________________
        virtual ~lockable() throw(); //!< cleanup

    protected:
        explicit lockable() throw(); //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(lockable);
        
    public:
        static lockable & giant(); //!< from concurrent::quark internal
    };

    //! create the guard name
#define YACK_LOCK__(X,Y) X##Y

    //! instantiace the guard name
#define YACK_LOCK_(HOST,ID) volatile yack::lockable::scope  YACK_LOCK__(__guard,ID)(HOST)

    //! use a local scoped_lock to lock HOST
#define YACK_LOCK(HOST) YACK_LOCK_(HOST,__LINE__)

    //! shared giant top level lock
#define YACK_GIANT_LOCK() YACK_LOCK(lockable::giant())

    
    //__________________________________________________________________________
    //
    //! user defined scope unlocking, to use after a try_lock
    //__________________________________________________________________________
    class return_unlocked
    {
    public:
        return_unlocked(lockable &) throw(); //!< do nothing
        ~return_unlocked()          throw(); //!< unlock

    private:
        lockable &host;
        YACK_DISABLE_COPY_AND_ASSIGN(return_unlocked);
    };

}

#endif
