//! \file

#ifndef YACK_DATA_ACTUAL_LIST_INCLUDED
#define YACK_DATA_ACTUAL_LIST_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    //!
    /**
     - NODE must be constructed with param_type
     - NODE must have a copy constructor
     - NODE must have a operator*
     */
    template <typename T, typename NODE>
    class klist : public cxx_list_of<NODE>
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef cxx_list_of<NODE> list_type;
        typedef NODE              node_type;
        using list_type::size;
        using list_type::head;
        using list_type::tail;

        inline explicit klist() throw() : list_type() {}
        inline virtual ~klist() throw() {}
        inline klist(const klist &other) : list_type(other) {}

        inline type & append_back(param_type args)  { return **(this->push_back( new NODE(args) )); }
        inline type & append_front(param_type args) { return **(this->push_front( new NODE(args) )); }

        inline type       & front()       throw() { assert(size>0); return **head; }
        inline const_type & front() const throw() { assert(size>0); return **head; }

        inline type       & back()       throw() { assert(size>0); return **tail; }
        inline const_type & back() const throw() { assert(size>0); return **tail; }

        inline void delete_back()  throw() { delete this->pop_back();  }
        inline void delete_front() throw() { delete this->pop_front(); }

        inline type pull_back()  { const_type temp = **tail; delete_back(); return temp; }
        inline type pull_front() { const_type temp = **head; delete_front(); return temp; }

        klist & operator<<(param_type   rhs)  { (void)(this->push_back( new NODE(rhs) )); return *this; }
        klist & operator<<(const klist &rhs)  { this->merge_back_copy(rhs); return *this; }
        inline klist & operator>>(param_type rhs) { (void)(this->push_front( new NODE(rhs) )); return *this; }
    };

}

#endif

