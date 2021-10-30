
//! \file

#ifndef YACK_COUNTING_INCLUDED
#define YACK_COUNTING_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! counting interface
    //
    //__________________________________________________________________________
    class counting
    {
    public:
        virtual     ~counting() throw();     //!< cleanup
        virtual void boot()     throw() = 0; //!< boot state
        virtual bool next()     throw() = 0; //!< next state
        
    protected:
        explicit counting() throw(); //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counting);
    };
    
}

#endif

