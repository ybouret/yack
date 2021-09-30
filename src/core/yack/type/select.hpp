//! \file

#ifndef YACK_TYPE_SELECT_INCLUDED
#define YACK_TYPE_SELECT_INCLUDED 1

#include "yack/setup.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //! default type selector.
    //__________________________________________________________________________
    template <const bool flag, typename T, typename U> struct select_type;

    //__________________________________________________________________________
    //
    //! type selector when true => T
    //__________________________________________________________________________
    template <typename T, typename U>
    struct select_type<true,T,U>
    {
        typedef T result; //!< flag is true
    };

    //__________________________________________________________________________
    //
    //! type selector when false => U
    //__________________________________________________________________________
    template <typename T, typename U>
    struct select_type<false,T,U>
    {
        typedef U result; //!< flag is false
    };
}

#endif

