//! \file
#ifndef YACK_IPOWER_INCLUDED
#define YACK_IPOWER_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //! ipower
    template <typename T,typename U>
    static inline T ipower(const T x, U n)
    {
        assert(n>=0);
        if(n<=0)
        {
            return T(1);
        }
        else
        {
            T res = x;
            while(--n > 0)
            {
                res *= x;
            }
            return res;
        }
    }

}

#endif

