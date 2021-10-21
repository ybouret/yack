//! \file

#ifndef YACK_DATA_SMALL_NODE_INCLUDED
#define YACK_DATA_SMALL_NODE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    template <typename T>
    class small_node : public object
    {
    public:
        YACK_DECL_ARGS(T,type);

        small_node *next;
        small_node *prev;


        inline  small_node(param_type args) : next(0), prev(0), data(args) {}
        inline ~small_node() throw() { assert(NULL==next); assert(NULL==prev); }

        inline type       & operator*() throw()       { return data; }
        inline const_type & operator*() const throw() { return data; }

    private:
        YACK_DISABLE_ASSIGN(small_node);
        type data;
    };
    
}

#endif

