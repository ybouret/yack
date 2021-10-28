//! \file

#ifndef YACK_DYNAMIC_INCLUDED
#define YACK_DYNAMIC_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base class for class with granted memory
    //
    //__________________________________________________________________________
    class dynamic
    {
    public:
        virtual size_t granted() const throw() = 0; //!< linear space
        virtual       ~dynamic() throw();           //!< cleanup

    protected:
        explicit       dynamic() throw();           //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dynamic);
    };

}

#endif

