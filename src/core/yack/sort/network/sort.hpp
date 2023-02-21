
//! \file

#ifndef YACK_NETWORK_SORT_INCLUDED
#define YACK_NETWORK_SORT_INCLUDED 1

#include "yack/sort/network/sort2.hpp"
#include "yack/sort/network/sort3.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/sort/network/sort6.hpp"
#include "yack/sort/network/sort7.hpp"
#include "yack/sort/network/sort8.hpp"
#include "yack/sort/network/sort9.hpp"
#include "yack/sort/network/sort10.hpp"
#include "yack/sort/network/sort11.hpp"
#include "yack/sort/network/sort12.hpp"
#include "yack/sort/network/sort13.hpp"
#include "yack/sort/network/sort14.hpp"
#include "yack/sort/network/sort15.hpp"
#include "yack/sort/network/sort16.hpp"


#include "yack/sequence/thin-array.hpp"
#include "yack/type/utils.hpp"
#include "yack/singleton.hpp"

#include <iostream>

namespace yack {

    //__________________________________________________________________________
    //
    //
    // macro to help build algorithm
    //
    //__________________________________________________________________________

    //! helper to declare a predefine network
#define YACK_NETWORK_SORT_DECLARE(N) const network_sort##N s##N

    //! helper to initialize algorithms
#define YACK_NETWORK_SORT_IMPL(N)  s##N()

    //! repeat macro according to implementation
#define YACK_NETWORK_SORT_REP(MACRO,SEP) \
YACK_NETWORK_SORT_##MACRO(2)  SEP \
YACK_NETWORK_SORT_##MACRO(3)  SEP \
YACK_NETWORK_SORT_##MACRO(4)  SEP \
YACK_NETWORK_SORT_##MACRO(5)  SEP \
YACK_NETWORK_SORT_##MACRO(6)  SEP \
YACK_NETWORK_SORT_##MACRO(7)  SEP \
YACK_NETWORK_SORT_##MACRO(8)  SEP \
YACK_NETWORK_SORT_##MACRO(9)  SEP \
YACK_NETWORK_SORT_##MACRO(10) SEP \
YACK_NETWORK_SORT_##MACRO(11) SEP \
YACK_NETWORK_SORT_##MACRO(12) SEP \
YACK_NETWORK_SORT_##MACRO(13) SEP \
YACK_NETWORK_SORT_##MACRO(14) SEP \
YACK_NETWORK_SORT_##MACRO(15) SEP \
YACK_NETWORK_SORT_##MACRO(16)


    //! local increasing macro
#define YACK_NETWORK_SORT_DISPATCH_INCR(N) \
case (N-1): { thin_array<typename ARRAY::mutable_type> data( &arr[lo], N ); s##N.increasing(data); return; }

    //! local decreasing macro
#define YACK_NETWORK_SORT_DISPATCH_DECR(N) \
case (N-1): { thin_array<typename ARRAY::mutable_type> data( &arr[lo], N ); s##N.decreasing(data); return; }

    //! local decreasing abs macro
#define YACK_NETWORK_SORT_DISPATCH_DECR_ABS(N) \
case (N-1): { thin_array<typename ARRAY::mutable_type> data( &arr[lo], N ); s##N.decreasing_abs(data); return; }


    //__________________________________________________________________________
    //
    //
    //! instance of sorting algorithm by default comparison
    //
    //__________________________________________________________________________
    class network_sort : public singleton<network_sort>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const at_exit::longevity life_time = 4000; //!< singleton life_time
        static const char * const       call_sign;        //!< singleton call sign



        //______________________________________________________________________
        //
        //
        // methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! sort in increasing order
        //______________________________________________________________________
        template <typename ARRAY> inline
        void increasing(ARRAY &arr) const noexcept
        {
            quick_sort_incr(arr,1,arr.size());
        }

        //______________________________________________________________________
        //
        //! sort in decreasing order
        //______________________________________________________________________
        template <typename ARRAY> inline
        void decreasing(ARRAY &arr) const noexcept
        {
            quick_sort_decr(arr,1,arr.size());
        }

        //______________________________________________________________________
        //
        //! sort in absolute value decreasing order
        //______________________________________________________________________
        template <typename ARRAY> inline
        void decreasing_abs(ARRAY &arr) const noexcept
        {
            quick_sort_decr_abs(arr,1,arr.size());
        }




        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        YACK_NETWORK_SORT_REP(DECLARE,;); //!< initialized sorting algorithms

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(network_sort);
        friend class singleton<network_sort>;
        explicit network_sort() noexcept;
        virtual ~network_sort() noexcept;

        
        
        //----------------------------------------------------------------------
        //
        // increasing order
        //
        //----------------------------------------------------------------------
        template <typename ARRAY> static inline
        size_t partition_incr( ARRAY &arr, const size_t lo, const size_t hi) noexcept
        {
            assert(hi>=lo);

            typename ARRAY::const_type pivot = arr[ (lo+hi)>>1 ];
            size_t  i     = lo-1;
            size_t  j     = hi+1;
            while(true)
            {
                do ++i; while ( arr[i]<pivot );
                do --j; while ( arr[j]>pivot );
                if(i>=j) return j;
                cswap(arr[i],arr[j]);
            }
        }

        template <typename ARRAY>  inline
        void dispatch_incr(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(hi>=lo);
            switch(hi-lo)
            {
                case 0:  return; YACK_NETWORK_SORT_REP(DISPATCH_INCR,;);
                default: quick_sort_incr(arr,lo,hi);
            }
        }

        template <typename ARRAY>   inline
        void quick_sort_incr(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(lo>0);
            if(lo<hi)
            {
                const size_t p = partition_incr(arr,lo,hi);
                dispatch_incr(arr,lo,p);
                dispatch_incr(arr,p+1,hi);
            }
        }


        //----------------------------------------------------------------------
        //
        // decreasing order
        //
        //----------------------------------------------------------------------

        template <typename ARRAY> static inline
        size_t partition_decr(ARRAY &arr, const size_t lo, const size_t hi) noexcept
        {
            typename ARRAY::const_type pivot = arr[ (lo+hi)>>1 ];
            size_t  i     = lo-1;
            size_t  j     = hi+1;
            while(true)
            {
                do ++i; while ( arr[i]>pivot );
                do --j; while ( arr[j]<pivot );
                if(i>=j) return j;
                cswap(arr[i],arr[j]);
            }
        }

        template <typename ARRAY> inline
        void dispatch_decr(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(hi>=lo);
            switch(hi-lo)
            {
                case 0:  return; YACK_NETWORK_SORT_REP(DISPATCH_DECR,;);
                default: quick_sort_decr(arr,lo,hi);
            }
        }

        template <typename ARRAY> inline
        void quick_sort_decr(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(lo>0);
            if(lo<hi)
            {
                const size_t p = partition_decr(arr,lo,hi);
                dispatch_decr(arr,lo,p);
                dispatch_decr(arr,p+1,hi);
            }
        }



        //----------------------------------------------------------------------
        //
        // abs decreasing order
        //
        //----------------------------------------------------------------------
        template <typename ARRAY> static inline
        size_t partition_decr_abs(ARRAY &arr, const size_t lo, const size_t hi) noexcept
        {
            typename ARRAY::const_type pivot = std::abs(arr[ (lo+hi)>>1 ]);
            size_t  i     = lo-1;
            size_t  j     = hi+1;
            while(true)
            {
                do ++i; while ( std::abs(arr[i])>pivot );
                do --j; while ( std::abs(arr[j])<pivot );
                if(i>=j) return j;
                cswap(arr[i],arr[j]);
            }
        }

        template <typename ARRAY>  inline
        void dispatch_decr_abs(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(hi>=lo);
            switch(hi-lo)
            {
                case 0:  return; YACK_NETWORK_SORT_REP(DISPATCH_DECR_ABS,;);
                default: quick_sort_decr_abs(arr,lo,hi);
            }
        }

        template <typename ARRAY>   inline
        void quick_sort_decr_abs(ARRAY &arr, const size_t lo, const size_t hi) const noexcept
        {
            assert(lo>0);
            if(lo<hi)
            {
                const size_t p = partition_decr_abs(arr,lo,hi);
                dispatch_decr_abs(arr,lo,p);
                dispatch_decr_abs(arr,p+1,hi);
            }
        }

    };

    
}

#endif



