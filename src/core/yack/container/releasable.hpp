
//! \file

#ifndef YACK_RELEASABLE_INCLUDED
#define YACK_RELEASABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! releasable base class
    //
    //__________________________________________________________________________
    class releasable
    {
    public:
        virtual     ~releasable() throw();     //!< cleanup
        virtual void release()    throw() = 0; //!< release resources

    protected:
        explicit releasable() throw();         //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(releasable);
    };

}

#endif
