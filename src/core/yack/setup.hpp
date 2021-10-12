//! \file

#ifndef YACK_SETUPXX_INCLUDED
#define YACK_SETUPXX_INCLUDED 1

#include "yack/system/setup.h"

//! disable copy constructor of CLASS
#define YACK_DISABLE_COPY(CLASS)   private: CLASS(const CLASS&)

//! disable assign operator for CLASS
#define YACK_DISABLE_ASSIGN(CLASS) private: CLASS & operator=(const CLASS &)

//! disable copy and assign for CLASS
#define YACK_DISABLE_COPY_AND_ASSIGN(CLASS) \
YACK_DISABLE_COPY(CLASS);                   \
YACK_DISABLE_ASSIGN(CLASS)

namespace yack
{

    //! matching size_t
    typedef ptrdiff_t unit_t;

    //! inline cswap of integral objects
    template <typename T> inline
    void cswap(T &lhs, T &rhs) throw()
    {
        const T tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

    //! inline type mutable coercion
    template <typename T> inline
    T & coerce(const T &source) throw() { return (T&)source;  }

    //! force cswap of integral objects
    template <typename T> inline
    void coerce_cswap(const T &lhs, const T &rhs) throw()
    {
        cswap( coerce(lhs), coerce(rhs) );
    }
}

#endif
