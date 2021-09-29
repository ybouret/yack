//! \file

#ifndef YACK_SETUPXX_INCLUDED
#define YACK_SETUPXX_INCLUDED 1

#include "yack/system/setup.h"

#define YACK_DISABLE_COPY(CLASS)   private: CLASS(const CLASS&)
#define YACK_DISABLE_ASSIGN(CLASS) private: CLASS & operator=(const CLASS &)

#define YACK_DISABLE_COPY_AND_ASSIGN(CLASS) \
YACK_DISABLE_COPY(CLASS);                   \
YACK_DISABLE_ASSIGN(CLASS)

namespace yack
{
    template <typename T> inline
    void cswap(T &lhs, T &rhs) throw()
    {
        const T tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

}

#endif
