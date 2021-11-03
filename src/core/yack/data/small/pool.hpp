//! \file

#ifndef YACK_DATA_SMALL_POOL_INCLUDED
#define YACK_DATA_SMALL_POOL_INCLUDED 1

#include "yack/data/small/node.hpp"
#include "yack/data/pool/concrete.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! conrete pool with nodes based on object::memory
    //
    //__________________________________________________________________________
    template <typename T>
    class small_pool : public kpool< T, small_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef small_node<T>              node_type; //!< alias
        typedef kpool<T,node_type>         self_type; //!< alias
        using self_type::head;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit small_pool() throw() : self_type() {} //!< setup
        inline virtual ~small_pool() throw() {}               //!< cleanup
        inline          small_pool(const small_pool &other) : self_type(other) {} //!< hard copy

        
        
    private:
        YACK_DISABLE_ASSIGN(small_pool);
    };

}

#endif
