
//! \file

#ifndef YACK_MSWAP_INCLUDED
#define YACK_MSWAP_INCLUDED 1

#include "yack/type/out-of-reach.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //! inline mswap of POD type
    //__________________________________________________________________________
    template <typename T> inline
    void mswap(T &lhs, T &rhs) noexcept
    {
        uint8_t   *p = static_cast<uint8_t *>( out_of_reach::address(&lhs) );
        uint8_t   *q = static_cast<uint8_t *>( out_of_reach::address(&rhs) );
        for(size_t i=0;i<sizeof(T);++i)
        {
            cswap(p[i],q[i]);
        }
    }


}

#endif
