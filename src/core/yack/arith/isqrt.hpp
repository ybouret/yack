//! \file

#ifndef YACK_ISQRT_INCLUDED
#define YACK_ISQRT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //! integer square root
    template <typename T> inline
    T isqrt(const T s) throw()
    {
        if(s<=1) return s;

        T x0 = s >> 1 ;

        while(true) {
            const T x1 = (x0+s/x0)>>1;
            if(x1>=x0) break;
            x0 = x1;
        }
        return x0;
        
#if 0
        T x0 = s  ;
        T x1 = ( x0 + s / x0 ) >> 1;

        while ( x1 < x0 )
        {
            x0 = x1;
            x1 = ( x0 + s / x0 ) >>1;
        }
        return x0;
#endif
    }


}

#endif

