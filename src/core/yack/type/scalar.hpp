
//! \file

#ifndef YACK_TYPE_SCALAR_INCLUDED
#define YACK_TYPE_SCALAR_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //! scalar type for
    template <typename T>
    struct scalar_for
    {
        typedef T type; //!< default
    };

}

#endif
