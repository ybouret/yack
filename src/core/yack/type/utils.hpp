//! \file

#ifndef YACK_TYPE_UTILS_INCLUDED
#define YACK_TYPE_UTILS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //! min of (a,b)
    template <typename T> inline
    T min_of(const T lhs, const T rhs) noexcept
    {
        return (lhs < rhs) ? lhs : rhs;
    }

    //! max of (a,b)
    template <typename T> inline
    T max_of(const T lhs, const T rhs) noexcept
    {
        return (lhs < rhs) ? rhs : lhs;
    }

    //! max of (a,b,c)
    template <typename T> inline
    T max_of(const T a, const T b, const T c) noexcept
    {
        return max_of(a,max_of(b,c));
    }



    //! keep within [lower:upper]
    template <typename T> inline
    T clamp(const T lower, const T value, const T upper) noexcept
    {
        return (value<lower) ? lower : ( (upper<value) ? upper : value );
    }
    
    //! x^2
    template <typename T> inline
    T squared(const T x) noexcept
    {
        return x*x;
    }

    //! x^3
    template <typename T> inline
    T cubed(const T x) noexcept
    {
        return x*x*x;
    }

    //! x *= x, in situ
    template <typename T> inline
    void square(T &x) noexcept
    {
        x *= x;
    }

    //! x *= x in situ, return x
    template <typename T> inline
    T square_pop(T &x) noexcept
    {
        const T ans = x;
        x *= x;
        return ans;
    }

    //! x+x
    template <typename T> inline
    T twice(const T x) noexcept
    {
        return x+x;
    }


    //! x = -x
    template <typename T> inline
    void opposite(T &x) noexcept {
        x = -x;
    }
    
}

#endif
