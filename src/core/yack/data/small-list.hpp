
//! \file

#ifndef YACK_DATA_SMALL_LIST_INCLUDED
#define YACK_DATA_SMALL_LIST_INCLUDED 1

#include "yack/data/small-node.hpp"
#include "yack/data/cxx-list.hpp"

namespace yack
{

    template <typename T>
    class small_list : public cxx_list_of< small_node<T> >
    {
    };

}

#endif
