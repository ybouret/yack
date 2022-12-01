
//! \file

#ifndef YACK_SEQUENCE_ROLL_INCLUDED
#define YACK_SEQUENCE_ROLL_INCLUDED 1

#include "yack/container/writable.hpp"

namespace yack
{

    //! rolling algorithm(s)
    struct rolling
    {
        //! roll down an array
        template <typename T> static inline
        void down(writable<T> &arr)
        {
            const size_t n = arr.size();
            switch (n) {
                case 0:
                case 1:  return;
                case 2:  cswap(arr[1],arr[2]); return;
                default: break;
            }
            typename writable<T>::const_type arr1 = arr[1];
            for(size_t ip=1,i=2;i<=n;++i,++ip)
            {
                arr[ip] = arr[i];
            }
            arr[n] = arr1;
        }


    };

}


#endif
