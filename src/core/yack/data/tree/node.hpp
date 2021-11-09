//! \file

#ifndef YACK_DATA_TREE_NODE_INCLUDED
#define YACK_DATA_TREE_NODE_INCLUDED 1

#include "yack/data/tree/knot.hpp"

namespace yack
{

    template <typename T, typename CODE>
    class tree_node : public object
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef tree_node<T,CODE>       node_type;
        typedef tree_knot<T,node_type>  knot_type;
        typedef cxx_list_of<node_type>  list_type;
        typedef cxx_pool_of<node_type>  pool_type;

        inline explicit tree_node(const CODE c) throw() :
        code(c), next(NULL), prev(NULL), chld(), knot(NULL)
        {
        }

        inline virtual ~tree_node() throw()
        {

        }

        inline bool is_filled() const throw() { return NULL != knot; }
        inline bool is_vacant() const throw() { return NULL == knot; }

        inline type       & operator*()       throw() { assert(knot); assert(knot->is_alive()); return **knot; }
        inline const_type & operator*() const throw() { assert(knot); assert(knot->is_alive()); return **knot; }


        const CODE code;
        tree_node *next;
        tree_node *prev;
        list_type  chld;

    private:
        knot_type  *knot;
        YACK_DISABLE_COPY_AND_ASSIGN(tree_node);

    };

}

#endif

