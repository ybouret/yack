
//! \file
#ifndef YACK_SORT_HEAP_INCLUDED
#define YACK_SORT_HEAP_INCLUDED 1

#include "yack/container/writable.hpp"
#include "yack/code/mmove.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace core
    {
        //______________________________________________________________________
        //
        //
        //! heap sort for more than 2 items
        //
        //______________________________________________________________________
        template <typename T,typename FUNC>
        inline void heap_sort(addressable<T> &ra,
                              FUNC           &compare)
        {
            const size_t n = ra.size();
            assert(n>=2);

            //----------------------------------------------------------------------
            // local memory
            //----------------------------------------------------------------------
            char wksp[ Y_MEMORY_ALIGN(sizeof(T)) ]; assert(sizeof(wksp)>=sizeof(T));
            T   &rra = *aliasing::as<T>( &wksp[0] );

            //----------------------------------------------------------------------
            // algorithm
            //----------------------------------------------------------------------
            size_t l =(n >> 1)+1;
            size_t ir=n;
            for (;;)
            {
                if (l>1)
                {
                    bmove(rra,ra[--l]);      assert( bsame(rra,ra[l]) );
                }
                else
                {
                    bmove( rra,    ra[ir] ); assert( bsame(rra,   ra[ir]) );
                    bmove( ra[ir], ra[1]  ); assert( bsame(ra[ir],ra[1])  );
                    if (--ir == 1)
                    {
                        bmove(ra[1],rra);    assert( bsame(ra[1],rra) );
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
                        bmove( ra[i], ra[j] ); assert( bsame( ra[i], ra[j] ) );
                        i=j;
                        (j<<=1);
                    }
                    else
                    {
                        j=ir+1;
                    }
                }
                bmove( ra[i], rra ); assert( bsame( ra[i], rra ) );
            }
        }
    }

    //__________________________________________________________________________
    //
    //
    //! heap sort
    //
    //__________________________________________________________________________
    template <typename T,typename FUNC>
    inline void hsort(addressable<T> &ra,
                      FUNC           &compare)
    {
        switch( ra.size() )
        {
            case 0:
            case 1:
                return;

            default:
                core::heap_sort(ra,compare); return;
        }
    }

#if 0
    //__________________________________________________________________________
    //
    //
    //! heap sort local data ra[0..na-1]
    //
    //__________________________________________________________________________
    template <typename T,typename FUNC>
    inline void hsort(T              *ra,
                      const size_t    na,
                      FUNC           &compare )
    {
        switch( na )
        {
            case 1: assert(NULL!=ra);
            case 0:
                return;

            default:
                assert(NULL!=ra);
                lightweight_array<T> arr(ra,na);
                core::heap_sort(arr,compare); return;
        }
    }

#endif


    //__________________________________________________________________________
    //
    //
    //! heap co sort
    //
    //__________________________________________________________________________
    template <typename T, typename U, typename FUNC>
    inline void hsort(addressable<T> &ra, addressable<U> &rb, FUNC &compare)
    {
        assert( ra.size() == rb.size() );
        const size_t n = ra.size();
        if (n<2) return;

        //----------------------------------------------------------------------
        //-- local memory
        //----------------------------------------------------------------------
        char wksp[ Y_MEMORY_ALIGN(sizeof(T)) ]; assert(sizeof(wksp)>=sizeof(T));
        T   &rra = *aliasing::as<T>( &wksp[0] );

        char wksp2[ Y_MEMORY_ALIGN(sizeof(U)) ]; assert(sizeof(wksp2)>=sizeof(U));
        U   &rrb = *aliasing::as<U>( &wksp2[0] );

        //----------------------------------------------------------------------
        //-- algorithm
        //----------------------------------------------------------------------
        size_t l =(n >> 1)+1;
        size_t ir=n;
        for (;;)
        {
            if (l>1)
            {
                --l;
                bmove(rra,ra[l]);
                bmove(rrb,rb[l]);
            }
            else
            {
                bmove( rra,    ra[ir] );
                bmove( ra[ir], ra[1]  );

                bmove( rrb,    rb[ir] );
                bmove( rb[ir], rb[1]  );

                if (--ir == 1)
                {
                    bmove(ra[1],rra);
                    bmove(rb[1],rrb);
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
                    bmove( ra[i], ra[j] );
                    bmove( rb[i], rb[j] );
                    i=j;
                    (j <<= 1);
                }
                else
                {
                    j=ir+1;
                }
            }
            bmove( ra[i], rra );
            bmove( rb[i], rrb );
        }
    }
}
#endif

