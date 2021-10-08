//! \file

#ifndef YACK_COMPARISON_INCLUDED
#define YACK_COMPARISON_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! generic comparison function
    //
    //__________________________________________________________________________
    struct comparison
    {
        //______________________________________________________________________
        //
        //! increasiong
        //______________________________________________________________________
        template <typename T> static inline
        int increasing(const T &lhs, const T &rhs) throw()
        {
            return (lhs<rhs) ? -1 : (rhs<lhs ? 1 : 0);
        }
        
        //______________________________________________________________________
        //
        //! decreasing
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing(const T &lhs, const T &rhs) throw()
        {
            return (lhs<rhs) ? 1 : (rhs<lhs ? -1 : 0);
        }
        
        
    };
    
}

#endif

