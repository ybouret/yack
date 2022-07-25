
//! \file

#ifndef YACK_NETWORK_SORT_INCLUDED
#define YACK_NETWORK_SORT_INCLUDED 1

#include "yack/sort/network/sort2.hpp"
#include "yack/sort/network/sort10.hpp"

namespace yack {
    
     
        struct sort_by
        {
            template <typename ARRAY>
            void perform(ARRAY &arr)
            {
                static const network_sort2  s2;
                static const network_sort2  s3;
                static const network_sort10 s10;
                const size_t n = arr.size();
                switch(n)
                {
                    case 0: return;
                    case 1: return;
                    case 2: s2.increasing(arr); return;
                    case 3: s3.increasing(arr); return;
                    default:
                        ;
                }
            }
            
        };
     
    
}

#endif



