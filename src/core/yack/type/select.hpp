//! \file

#ifndef YACK_TYPE_SELECT_INCLUDED
#define YACK_TYPE_SELECT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! default type selector.
    //__________________________________________________________________________
    template <const bool flag, typename T, typename U> struct select_from;

    //__________________________________________________________________________
    //
    //! type selector when true => T
    //__________________________________________________________________________
    template <typename T, typename U>
    struct select_from<true,T,U>
    {
        typedef T result; //!< flag is true
    };

    //__________________________________________________________________________
    //
    //! type selector when false => U
    //__________________________________________________________________________
    template <typename T, typename U>
    struct select_from<false,T,U>
    {
        typedef U result; //!< flag is false
    };
}

#endif

