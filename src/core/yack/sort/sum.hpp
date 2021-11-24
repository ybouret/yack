
//! \file
#ifndef YACK_SORT_SUM_INCLUDED
#define YACK_SORT_SUM_INCLUDED 1

#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"

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
        
    };



}

#endif
