
//! \file

#ifndef YACK_SORT_NETWORK_INCLUDED
#define YACK_SORT_NETWORK_INCLUDED

#include "yack/sort/nwswp.hpp"
#include "yack/type/out-of-reach.hpp"
namespace yack
{
    
    struct network_sort
    {
      
        template <typename ARRAY> static inline
        void increasing(ARRAY &arr, const nwsrt::swaps &swp)
        {
            assert(arr.size()==swp.size);
            const size_t  nswp = swp.work;
            const size_t *ltab = swp.ltab;
            const size_t *rtab = swp.rtab;
            for(size_t k=0;k<nswp;++k)
            {
                typename ARRAY::type &l = arr[ ltab[k] ];
                typename ARRAY::type &r = arr[ rtab[k] ];
                if(r<l) out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
             }
        }
        
    };
    
}

#endif
