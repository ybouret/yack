//! \file

#ifndef YACK_DATA_SMALL_NODE_INCLUDED
#define YACK_DATA_SMALL_NODE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! node based on object::memory to hold a single (integral) type
    //
    //__________________________________________________________________________
    template <typename T>
    class small_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  small_node(param_type args) : next(0), prev(0), data(args) {}                       //!< setup
        inline ~small_node() throw() { assert(NULL==next); assert(NULL==prev); }                    //!< cleanup
        inline  small_node(const small_node &node) : object(), next(0), prev(0), data(node.data) {} //!< hard copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline type       & operator*() throw()       { return data; } //!< access
        inline const_type & operator*() const throw() { return data; } //!< access, const

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        small_node *next; //!< for list/pool
        small_node *prev; //!< for list

    private:
        YACK_DISABLE_ASSIGN(small_node);
        type data;
    };
    
}

#endif

