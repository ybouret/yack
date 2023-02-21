
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
        virtual     ~releasable() noexcept;     //!< cleanup
        virtual void release()    noexcept = 0; //!< release resources

    protected:
        explicit releasable() noexcept;         //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(releasable);
    };

}

#endif
