
//! \file

#ifndef YACK_DATA_NODES_COMPARISON_INCLUDED
#define YACK_DATA_NODES_COMPARISON_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    template <typename NODE, typename COMPARE_DATA>
    struct nodes_comparison
    {
        COMPARE_DATA &compare_data;
        inline int operator()(const NODE *lhs, const NODE *rhs)
        {
            assert(lhs);
            assert(rhs);
            return compare_data( **lhs, **rhs );
        }
    };
}

#endif

