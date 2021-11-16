//! \file

#ifndef YACK_DATA_HASH_TABLE_INCLUDED
#define YACK_DATA_HASH_TABLE_INCLUDED 1

#include "yack/data/hash/node.hpp"


namespace yack
{

    template <typename T>
    class hash_table
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef hash_node<T>                  node_type;  //!< node in the tree
        typedef list_of<node_type>            slot_type;  //!<
        typedef typename node_type::pool_type node_pool;  //!< pool of nodes

        typedef data_knot<T,node_type>        knot_type;  //!< knot of data
        typedef typename knot_type::list_type knot_list;  //!< list of knots
        typedef typename knot_type::pool_type knot_pool;  //!< pool of knots

        inline explicit hash_table() throw() :
        data(), slot(0), slots(1), smask(0), pool(), repo(), base()
        {
            slot = &base;
        }

        inline virtual ~hash_table() throw()
        {
            
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_table);
        knot_list  data;
        slot_type *slot;
        size_t     slots; //!< a power of two: 1 => use base
        size_t     smask; //!< slots-1
        knot_pool pool;
        node_pool repo;
        slot_type base;

    };

}

#endif

