//! \file

#ifndef YACK_DATA_SMALL_POOL_INCLUDED
#define YACK_DATA_SMALL_POOL_INCLUDED 1

#include "yack/data/small/node.hpp"
#include "yack/data/cxx-pool.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! pool with nodes based on object::memory
    //
    //__________________________________________________________________________
    template <typename T>
    class small_pool : public cxx_pool_of< small_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef small_node<T>                  node_type; //!< alias
        typedef cxx_pool_of<node_type>         self_type; //!< alias
        using self_type::head;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit small_pool() throw() : self_type() {} //!< setup
        inline virtual ~small_pool() throw() {}               //!< cleanup
        inline          small_pool(const small_pool &other) : self_type(other) {} //!< hard copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline void append(param_type args) { this->store( new node_type(args) ); }                       //!< create node/store
        inline type remove() throw()        { const_type tmp = **head; delete this->query(); return tmp;} //!< query value

        
    private:
        YACK_DISABLE_ASSIGN(small_pool);
    };

}

#endif
