//! \file

#ifndef YACK_TYPE_UTILS_INCLUDED
#define YACK_TYPE_UTILS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    template <typename T> static inline
    T min_of(const T lhs, const T rhs) throw()
    {
        return (lhs < rhs) ? lhs : rhs;
    }


    template <typename T> static inline
    T max_of(const T lhs, const T rhs) throw()
    {
        return (lhs < rhs) ? rhs : lhs;
    }
    
    template <typename T> static inline
    T square_of(const T x) throw()
    {
        return x*x;
    }
    
    

}

#endif
