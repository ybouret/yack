
//! \file

#ifndef YACK_DATA_HASH_NODE_INCLUDED
#define YACK_DATA_HASH_NODE_INCLUDED 1

#include "yack/data/knot.hpp"


namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! node within a hash table
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class hash_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                        //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                  //!< aliases
        typedef hash_node<KEY,T>            node_type; //!< alias
        typedef data_knot<T,node_type>      knot_type; //!< alias
        typedef cxx_list_of<node_type>      list_type; //!< alias
        typedef cxx_pool_of<node_type>      pool_type; //!< aluas

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit hash_node() throw() :
        next(NULL), prev(NULL), hkey(0), knot(NULL) {}
        
        //! cleanup
        inline virtual ~hash_node() throw() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline type       & operator*()       throw() { assert(knot); assert(knot->is_alive()); return **knot; } //!< access data
        inline const_type & operator*() const throw() { assert(knot); assert(knot->is_alive()); return **knot; } //!< access data, condt

        

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        hash_node        *next; //!< for list/pool
        hash_node        *prev; //!< for list
        const size_t      hkey; //!< for slots
        knot_type        *knot; //!< NULL:vacant, otherwise alive

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_node);

    };

}

#endif

