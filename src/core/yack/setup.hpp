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
    void cswap(T &lhs, T &rhs) noexcept
    {
        const T tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

    
    //! inline type mutable coercion
    template <typename T> inline
    T & coerce(const T &source) noexcept { return (T&)source;  }

    //! force cswap of integral objects
    template <typename T> inline
    void coerce_cswap(const T &lhs, const T &rhs) noexcept
    {
        cswap( coerce(lhs), coerce(rhs) );
    }

    //! helper for no-throw swap methods using 'other' args
#define YACK_EXCHANGE(FIELD) coerce_cswap(FIELD,other.FIELD)

    //! make a class from a type
    template <typename T>
    struct type2type
    {
        typedef T type; //!< original type
    };
    
    //! make a class from an integer value
    template <int N>
    struct int2type
    {
        static const int value = N; //!< original value
    };

    //! make a class from an unsigned value
    template <unsigned N>
    struct uint2type
    {
        static const unsigned value = N; //!< original value
    };

    
}

#endif
