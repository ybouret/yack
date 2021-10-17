
//! \file

#ifndef YACK_FFT_XBITREV_INCLUDED
#define YACK_FFT_XBITREV_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/type/constants.hpp"

#define  YACK_FFT_TRACK
#if defined(YACK_FFT_TRACK)
#include "yack/system/wtime.hpp"
#endif

#include <iostream>

namespace yack
{
    //! inline cswap of integral objects
    template <typename T> inline
    void cswap2(T *lhs, T *rhs) throw()
    {
        assert(lhs); assert(rhs);
        const T tmp0 = lhs[0];
        const T tmp1 = lhs[1];
        lhs[0] = rhs[0];
        lhs[1] = rhs[1];
        rhs[0] = tmp0;
        rhs[1] = tmp1;
    }
}

#include "yack/fft/xbr-decl.hxx"



#endif
