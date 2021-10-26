
//! \file
#ifndef YACK_SORT_HEAP_INCLUDED
#define YACK_SORT_HEAP_INCLUDED 1

#include "yack/container/writable.hpp"
#include "yack/type/mmove.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    
    //______________________________________________________________________
    //
    //
    //! heap sort for more than 2 items
    //
    //______________________________________________________________________
    template <typename T,typename FUNC>
    inline void hsort(writable<T> &ra,
                       FUNC        &compare)
    {
        const size_t n = ra.size();
        if(n<2) return;
        
        //----------------------------------------------------------------------
        // local memory
        //----------------------------------------------------------------------
        void *arr[ YACK_WORDS_FOR(T) ]; assert(sizeof(arr)>=sizeof(T));
        T    &rra = *coerce_cast<T>(arr);
        
        //----------------------------------------------------------------------
        // algorithm
        //----------------------------------------------------------------------
        size_t l =(n >> 1)+1;
        size_t ir=n;
        for (;;)
        {
            if (l>1)
            {
                mmove(rra,ra[--l]);
            }
            else
            {
                mmove( rra,    ra[ir] );
                mmove( ra[ir], ra[1]  );
                if (--ir == 1)
                {
                    mmove(ra[1],rra);
                    break;
                }
            }
            size_t i=l;
            size_t j=l+l;
            while (j <= ir)
            {
                if (j < ir && compare(ra[j],ra[j+1]) < 0 )
                {
                    j++;
                }
                if ( compare(rra,ra[j]) < 0)
                {
                    mmove( ra[i], ra[j] );
                    i=j;
                    (j<<=1);
                }
                else
                {
                    j=ir+1;
                }
            }
            mmove( ra[i], rra );
        }
    }
    
    
    
    
    
    
    
    //__________________________________________________________________________
    //
    //
    //! heap co sort
    //
    //__________________________________________________________________________
    template <typename T, typename U, typename FUNC>
    inline void hsort(writable<T> &ra, writable<U> &rb, FUNC &compare)
    {
        assert( ra.size() == rb.size() );
        const size_t n = ra.size();
        if (n<2) return;
        
        //----------------------------------------------------------------------
        // local memory
        //----------------------------------------------------------------------
        void *arr[ YACK_WORDS_FOR(T) ]; assert(sizeof(arr)>=sizeof(T));
        T    &rra = *coerce_cast<T>(arr);
        
        void *brr[ YACK_WORDS_FOR(U) ]; assert(sizeof(brr)>=sizeof(T));
        U    &rrb = *coerce_cast<U>(brr);
        
        //----------------------------------------------------------------------
        // algorithm
        //----------------------------------------------------------------------
        size_t l =(n >> 1)+1;
        size_t ir=n;
        for (;;)
        {
            if (l>1)
            {
                --l;
                mmove(rra,ra[l]);
                mmove(rrb,rb[l]);
            }
            else
            {
                mmove( rra,    ra[ir] );
                mmove( ra[ir], ra[1]  );
                
                mmove( rrb,    rb[ir] );
                mmove( rb[ir], rb[1]  );
                
                if (--ir == 1)
                {
                    mmove(ra[1],rra);
                    mmove(rb[1],rrb);
                    break;
                }
            }
            size_t i=l;
            size_t j=l+l;
            while (j <= ir)
            {
                if(j < ir && compare(ra[j],ra[j+1]) < 0 )
                    j++;
                if( compare(rra,ra[j]) < 0)
                {
                    mmove( ra[i], ra[j] );
                    mmove( rb[i], rb[j] );
                    i=j;
                    (j <<= 1);
                }
                else
                {
                    j=ir+1;
                }
            }
            mmove( ra[i], rra );
            mmove( rb[i], rrb );
        }
    }
}


#endif

