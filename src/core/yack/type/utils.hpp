//! \file

#ifndef YACK_TYPE_UTILS_INCLUDED
#define YACK_TYPE_UTILS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //! min of (a,b)
    template <typename T> inline
    T min_of(const T lhs, const T rhs) throw()
    {
        return (lhs < rhs) ? lhs : rhs;
    }

    //! max of (a,b)
    template <typename T> inline
    T max_of(const T lhs, const T rhs) throw()
    {
        return (lhs < rhs) ? rhs : lhs;
    }
    
    //! x^2
    template <typename T> inline
    T squared(const T x) throw()
    {
        return x*x;
    }
    
    //! x+x
    template <typename T> inline
    T twice(const T x) throw()
    {
        return x+x;
    }
    
    //! |x|
    template <typename T> inline
    T absolute(const T x) throw()
    {
        return x < 0 ? -x : x;
    }
    

}

#endif
