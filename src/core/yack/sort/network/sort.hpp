
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
#include "yack/sequence/thin-array.hpp"

#include "yack/singleton.hpp"

#include <iostream>

namespace yack {

#define YACK_NETWORK_SORT_DECLARE(N) const network_sort##N s##N
#define YACK_NETWORK_SORT_REP(MACRO) \
YACK_NETWORK_SORT_##MACRO(2); \
YACK_NETWORK_SORT_##MACRO(3); \
YACK_NETWORK_SORT_##MACRO(4); \
YACK_NETWORK_SORT_##MACRO(5); \
YACK_NETWORK_SORT_##MACRO(6); \
YACK_NETWORK_SORT_##MACRO(7); \
YACK_NETWORK_SORT_##MACRO(8); \
YACK_NETWORK_SORT_##MACRO(9); \
YACK_NETWORK_SORT_##MACRO(10) \

#define YACK_NETWORK_SORT_DISPATCH_INCR(N) \
case (N-1): { thin_array<typename ARRAY::mutable_type> data( &arr[lo], N ); s##N.increasing(data); return; }

#define YACK_NETWORK_SORT_DISPATCH_DECR(N) \
case (N-1): { thin_array<typename ARRAY::mutable_type> data( &arr[lo], N ); s##N.decreasing(data); return; }

    class network_sort : public singleton<network_sort>
    {
    public:
        static const at_exit::longevity life_time = 4000;
        static const char * const       call_sign;



        YACK_NETWORK_SORT_REP(DECLARE);

        template <typename ARRAY> inline
        void increasing(ARRAY &arr) const throw()
        {
            quick_sort_incr(arr,1,arr.size());
        }

        template <typename ARRAY> inline
        void decreasing(ARRAY &arr) const throw()
        {
            quick_sort_decr(arr,1,arr.size());
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(network_sort);
        friend class singleton<network_sort>;
        explicit network_sort() throw();
        virtual ~network_sort() throw();

        template <typename ARRAY> static inline
        size_t partition_incr( ARRAY &arr, const size_t lo, const size_t hi) throw()
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
        void dispatch_incr(ARRAY &arr, const size_t lo, const size_t hi) const throw()
        {
            assert(hi>=lo);
            switch(hi-lo)
            {
                case 0:  return; YACK_NETWORK_SORT_REP(DISPATCH_INCR);
                default: quick_sort_incr(arr,lo,hi);
            }
        }

        template <typename ARRAY>   inline
        void quick_sort_incr(ARRAY &arr, const size_t lo, const size_t hi) const throw()
        {
            assert(lo>0);
            if(lo<hi)
            {
                const size_t p = partition_incr(arr,lo,hi);
                dispatch_incr(arr,lo,p);
                dispatch_incr(arr,p+1,hi);
            }
        }

        template <typename ARRAY> static inline
        size_t partition_decr(ARRAY &arr, const size_t lo, const size_t hi) throw()
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

        template <typename ARRAY>  inline
        void dispatch_decr(ARRAY &arr, const size_t lo, const size_t hi) const throw()
        {
            assert(hi>=lo);
            switch(hi-lo)
            {
                case 0:  return; YACK_NETWORK_SORT_REP(DISPATCH_DECR);
                default: quick_sort_decr(arr,lo,hi);
            }
        }

        template <typename ARRAY>   inline
        void quick_sort_decr(ARRAY &arr, const size_t lo, const size_t hi) const throw()
        {
            assert(lo>0);
            if(lo<hi)
            {
                const size_t p = partition_decr(arr,lo,hi);
                dispatch_decr(arr,lo,p);
                dispatch_decr(arr,p+1,hi);
            }
        }

    };

    
}

#endif



