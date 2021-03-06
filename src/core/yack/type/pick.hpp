//! \file

#ifndef YACK_TYPE_PICK_INCLUDED
#define YACK_TYPE_PICK_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! default type selector.
    //__________________________________________________________________________
    template <const bool flag, typename T, typename U> struct pick_from;

    //__________________________________________________________________________
    //
    //! type selector when true => T
    //__________________________________________________________________________
    template <typename T, typename U>
    struct pick_from<true,T,U>
    {
        typedef T type; //!< flag is true
    };

    //__________________________________________________________________________
    //
    //! type selector when false => U
    //__________________________________________________________________________
    template <typename T, typename U>
    struct pick_from<false,T,U>
    {
        typedef U type; //!< flag is false
    };
}

#endif

