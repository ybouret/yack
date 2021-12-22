
//! \file
#ifndef YACK_SORT_HEAP_INCLUDED
#define YACK_SORT_HEAP_INCLUDED 1

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
    template <typename ARRAY,typename FUNC>
    inline void hsort(ARRAY &ra,
                      const size_t n,
                      FUNC  &compare)
    {
        assert(n<=ra.size());
        if(n<2) return;

        //----------------------------------------------------------------------
        // local memory
        //----------------------------------------------------------------------
        typedef typename ARRAY::type type;
        void *arr[ YACK_WORDS_FOR(type) ]; assert(sizeof(arr)>=sizeof(type));
        type &rra = *coerce_cast<type>(arr);

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

    //______________________________________________________________________
    //
    //
    //! heap sort for more than 2 items
    //
    //______________________________________________________________________
    template <typename ARRAY,typename FUNC>
    inline void hsort(ARRAY &ra,
                      FUNC  &compare)
    {
        hsort(ra,ra.size(),compare);
    }
    
    
    
    
    //__________________________________________________________________________
    //
    //
    //! heap co sort
    //
    //__________________________________________________________________________
    template <typename ARR, typename BRR, typename FUNC>
    inline void hsort(ARR &ra, BRR &rb, FUNC &compare)
    {
        assert( ra.size() == rb.size() );
        const size_t n = ra.size();
        if (n<2) return;
        
        //----------------------------------------------------------------------
        // local memory
        //----------------------------------------------------------------------
        typedef typename ARR::type T;
        void *arr[ YACK_WORDS_FOR(T) ]; assert(sizeof(arr)>=sizeof(T));
        T    &rra = *coerce_cast<T>(arr);

        typedef typename BRR::type U;
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

