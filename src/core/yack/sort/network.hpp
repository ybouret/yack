
//! \file

#ifndef YACK_SORT_NETWORK_INCLUDED
#define YACK_SORT_NETWORK_INCLUDED

#include "yack/sort/nwswp.hpp"
#include "yack/type/out-of-reach.hpp"
namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! assembling algorithms from swaps
    //
    //__________________________________________________________________________
    struct network_sort
    {
      
        //! default increasing order
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
        
        //! default co-sorting in increasing order
        template <typename ARRAY, typename BRRAY> static inline
        void increasing(ARRAY &arr, BRRAY &brr, const nwsrt::swaps &swp)
        {
            assert(arr.size()==swp.size);
            assert(brr.size()==swp.size);
            const size_t  nswp = swp.work;
            const size_t *ltab = swp.ltab;
            const size_t *rtab = swp.rtab;
            for(size_t k=0;k<nswp;++k)
            {
                const size_t         il = ltab[k];
                const size_t         ir = rtab[k];
                typename ARRAY::type &l = arr[ il ];
                typename ARRAY::type &r = arr[ ir ];
                if(r<l)
                {
                    out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                    out_of_reach::swap(&brr[il],&brr[ir],sizeof(typename BRRAY::type));
                }
            }
        }
        
        
        
    };
    
}

#endif
