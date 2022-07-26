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

    //! keep within [lower:upper]
    template <typename T> inline
    T clamp(const T lower, const T value, const T upper) throw()
    {
        return (value<lower) ? lower : ( (upper<value) ? upper : value );
    }
    
    //! x^2
    template <typename T> inline
    T squared(const T x) throw()
    {
        return x*x;
    }

    //! x^3
    template <typename T> inline
    T cubed(const T x) throw()
    {
        return x*x*x;
    }

    //! x *= x, in situ
    template <typename T> inline
    void square(T &x) throw()
    {
        x *= x;
    }

    //! x *= x in situ, return x
    template <typename T> inline
    T square_pop(T &x) throw()
    {
        const T ans = x;
        x *= x;
        return ans;
    }

    //! x+x
    template <typename T> inline
    T twice(const T x) throw()
    {
        return x+x;
    }
    
    
}

#endif
