
//! \file

#ifndef YACK_TYPE_MMOVE_INCLUDED
#define YACK_TYPE_MMOVE_INCLUDED 1

#include "yack/type/out-of-reach.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! inline mmove of POD type
    //__________________________________________________________________________
    template <typename T> inline
    void mmove(T &lhs, const T &rhs) noexcept
    {
        out_of_reach::move(&lhs,&rhs,sizeof(T));
    }


}

#endif
