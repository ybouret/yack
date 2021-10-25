
//! \file

#ifndef YACK_TYPE_SCALAR_INCLUDED
#define YACK_TYPE_SCALAR_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! scalar type for
    //
    //__________________________________________________________________________
    template <typename T>
    struct scalar_for
    {
        typedef T type; //!< default is same type
    };

}

#endif
