
//! \file

#ifndef YACK_DATA_SMALL_LIST_INCLUDED
#define YACK_DATA_SMALL_LIST_INCLUDED 1

#include "yack/data/small/node.hpp"
#include "yack/data/cxx-list.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list with nodes based on object::memory
    //
    //__________________________________________________________________________
    template <typename T>
    class small_list : public cxx_list_of< small_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef small_node<T>                  node_type; //!< alias
        typedef cxx_list_of<node_type>         self_type; //!< alias
        using self_type::head;
        using self_type::tail;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit small_list() throw() : self_type() {} //!< setup
        inline virtual ~small_list() throw() {}               //!< cleanup
        inline          small_list(const small_list &other) : self_type(other) {} //!< hard copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline void append_back(param_type args) { this->push_back( new node_type(args) ); }                      //!< create node/push_back
        inline type remove_back() throw()        { const_type tmp = **tail; delete this->pop_back(); return tmp;} //!< pop_back value

        inline void append_front(param_type args) { this->push_front( new node_type(args) ); }                       //!< create node/push_front
        inline type remove_front() throw()        { const_type tmp = **head; delete this->pop_front(); return tmp; } //!< pop_front value
        
    private:
        YACK_DISABLE_ASSIGN(small_list);
    };

}

#endif
