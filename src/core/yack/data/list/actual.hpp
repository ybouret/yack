//! \file

#ifndef YACK_DATA_ACTUAL_LIST_INCLUDED
#define YACK_DATA_ACTUAL_LIST_INCLUDED 1

#include "yack/data/list/cxx.hpp"

namespace yack
{


    template <typename T, typename NODE>
    class klist : public cxx_list_of<NODE>
    {
    public:
        using cxx_list_of<NODE>::node_type;

    };

}

#endif

