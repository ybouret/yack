
//! \file

#ifndef YACK_DATA_SMALL_LIST_INCLUDED
#define YACK_DATA_SMALL_LIST_INCLUDED 1

#include "yack/data/small/node.hpp"
#include "yack/data/list/concrete.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list with nodes based on object::memory
    //
    //__________________________________________________________________________
    template <typename T>
    class small_list : public klist< T, small_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);               //!< aliases
        typedef small_node<T>      node_type; //!< alias
        typedef klist<T,node_type> self_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit small_list() throw() : self_type() {} //!< setup
        inline virtual ~small_list() throw() {}               //!< cleanup
        inline          small_list(const small_list &other) : self_type(other) {} //!< hard copy

        
    private:
        YACK_DISABLE_ASSIGN(small_list);
    };

}

#endif
