
//! \file

#ifndef YACK_DATA_COM_LIST_INCLUDED
#define YACK_DATA_COM_LIST_INCLUDED 1


#include "yack/data/list/joint.hpp"

namespace yack
{

    template <typename T>
    class com_node
    {
    public:
        YACK_DECL_ARGS(T,type);

        com_node() : next(0), prev(0), data() {}
        com_node(const com_node &node) : next(0), prev(0), data(node.data) {}
        com_node(param_type      args) : next(0), prev(0), data(args)      {}


        com_node *next;
        com_node *prev;

    private:
        YACK_DISABLE_ASSIGN(com_node);
        mutable_type data;
    };

    


}

#endif

