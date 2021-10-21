
//! \file

#ifndef YACK_DATA_SMALL_LIST_INCLUDED
#define YACK_DATA_SMALL_LIST_INCLUDED 1

#include "yack/data/small/node.hpp"
#include "yack/data/cxx-list.hpp"

namespace yack
{

    template <typename T>
    class small_list : public cxx_list_of< small_node<T> >
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef small_node<T>                  node_type;
        typedef cxx_list_of<node_type>         self_type;
        using self_type::head;
        using self_type::tail;


        inline explicit small_list() throw() : self_type() {}
        inline virtual ~small_list() throw() {}
        inline          small_list(const small_list &other) : self_type(other) {}

        inline void append_back(param_type args) { this->push_back( new node_type(args) ); }
        inline type remove_back() throw()        { const_type tmp = **tail; delete this->pop_back(); return tmp;}

        inline void append_front(param_type args) { this->push_front( new node_type(args) ); }
        inline type remove_front() throw()        { const_type tmp = **head; delete this->pop_front(); return tmp; }
        
    private:
        YACK_DISABLE_ASSIGN(small_list);
    };

}

#endif
