
//! \file

#ifndef YACK_RESTARTABLE_INCLUDED
#define YACK_RESTARTABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! restartable base class
    //
    //__________________________________________________________________________
    class restartable
    {
    public:
        virtual ~restartable() throw();     //!< cleanup
        virtual void restart() throw() = 0; //!< hard reset
        
    protected:
        explicit restartable() throw();    //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(restartable);
    };
    
}

#endif
