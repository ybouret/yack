
//! \file

#ifndef YACK_DATA_NODES_COMPARISON_INCLUDED
#define YACK_DATA_NODES_COMPARISON_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! wrapper to call data comparison from linked node
    //
    //__________________________________________________________________________
    template <typename NODE, typename COMPARE_DATA>
    struct nodes_comparison
    {
        COMPARE_DATA &compare_data; //!< user defined function

        //! call user function on **nodes
        inline int operator()(const NODE *lhs, const NODE *rhs)
        {
            assert(lhs);
            assert(rhs);
            return compare_data( **lhs, **rhs );
        }
    };
}

#endif

