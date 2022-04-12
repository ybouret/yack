
//! \file
#ifndef YACK_SORT_SUM_INCLUDED
#define YACK_SORT_SUM_INCLUDED 1

#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! performing sum with sorting data
    //
    //______________________________________________________________________
    struct sorted
    {
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef int2type<0>          by_value_t;      //!< alias
        typedef int2type<1>          by_abs_value_t;  //!< alias
        static const by_value_t      by_value;        //!< alias
        static const by_abs_value_t  by_abs_value;    //!< alias

        //______________________________________________________________________
        //
        // functions
        //______________________________________________________________________

        //! sum after sorting by value
        template <typename ARR> inline static
        typename ARR::mutable_type  sum(ARR &arr, const by_value_t &)
        {
            size_t i = arr.size();
            switch(i)
            {
                case  0: return 0;
                case  1: return arr[1];
                default:
                    hsort(arr,comparison::decreasing<typename ARR::mutable_type>);
            }
            typename ARR::mutable_type res = arr[i];
            while(--i>0) res += arr[i];
            return res;
        }

        //! sum after sorting by absolute value
        template <typename ARR> static inline
        typename ARR::mutable_type sum(ARR &arr, const by_abs_value_t &)
        {
            size_t i = arr.size();
            switch(i)
            {
                case  0: return 0;
                case  1: return arr[1];
                default:
                    hsort(arr,comparison::decreasing_abs<typename ARR::mutable_type>);
            }
            typename ARR::mutable_type res = arr[i];
            while(--i>0) res += arr[i];
            return res;
        }

        //! sum after sorting squares
        template <typename ARR> inline static
        typename ARR::mutable_type  sum_squared(ARR &arr)
        {
            size_t i = arr.size();
            switch(i)
            {
                case  0: return 0;
                case  1: return squared(arr[1]);
                default:
                    for(size_t j=i;j>0;--j) square(arr[j]);
                    hsort(arr,comparison::decreasing<typename ARR::mutable_type>);
            }
            typename ARR::mutable_type res = arr[i];
            while(--i>0) res += arr[i];
            return res;
        }

        //! dot product
        template <typename ARR,typename BRR,typename XRR> inline static
        typename ARR::mutable_type dot(ARR &arr, BRR &brr, XRR &xrr)
        {
            assert(brr.size()==brr.size());
            assert(xrr.size()==arr.size());
            for(size_t i=arr.size();i>0;--i) xrr[i] = arr[i] * brr[i];
            return sum(xrr,by_abs_value);
        }

        //! mod2
        template <typename ARR, typename XRR> inline static
        typename ARR::mutable_type mod2(ARR &arr, XRR &xrr)
        {
            assert(xrr.size()==arr.size());
            const size_t n = arr.size();
            switch(n)
            {
                case 0: return 0;
                case 1: return squared(arr[1]);
                default:
                    break;
            }
            for(size_t i=n;i>0;--i) xrr[i] = squared(arr[i]);
            return sum(xrr,by_value);

        }

        

        //! sorted sum of A,B,C
        template <typename T> static inline
        T sum(const T A, const T B, const T C)
        {
            T  arr[3] = { A, B, C };
            thin_array<T> tab(arr,sizeof(arr)/sizeof(arr[0]));
            return sum(tab,by_value);
        }

    };



}

#endif
