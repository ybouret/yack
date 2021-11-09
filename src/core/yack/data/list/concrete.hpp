//! \file

#ifndef YACK_DATA_ACTUAL_LIST_INCLUDED
#define YACK_DATA_ACTUAL_LIST_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/type/args.hpp"
#include "yack/data/nodes-comparison.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of C++ data
    //
    /**
     - NODE must be constructed with param_type
     - NODE must have a copy constructor
     - NODE must have a operator*
     */
    //__________________________________________________________________________
    template <typename T, typename NODE>
    class klist : public cxx_list_of<NODE>
    {
    public:
        YACK_DECL_ARGS(T,type);               //!< aliases
        typedef cxx_list_of<NODE> list_type;  //!< alias
        typedef NODE              node_type;  //!< alias
        using list_type::size;
        using list_type::head;
        using list_type::tail;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit klist() throw() : list_type() {}       //!< setup empty
        inline virtual ~klist() throw() {}                     //!< cleanup
        inline klist(const klist &other) : list_type(other) {} //!< hard-copy
        //!assign by copy/swap
        inline klist & operator=(const klist &other) {
            klist tmp(other);
            this->swap_with(tmp);
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! simple push back helper
        inline void add(param_type args) { this->push_back( new NODE(args) ); }

        //! push_back new node
        inline type & append_back(param_type args)  { return **(this->push_back(  new NODE(args) )); }

        //! push_front new node
        inline type & append_front(param_type args) { return **(this->push_front( new NODE(args) )); }

        
        inline type       & front()       throw() { assert(size>0); return **head; } //!< get front item
        inline const_type & front() const throw() { assert(size>0); return **head; } //!< get front item, const

        inline type       & back()       throw() { assert(size>0); return **tail; }  //!< get back item
        inline const_type & back() const throw() { assert(size>0); return **tail; }  //!< get back item, const

        inline void delete_back()  throw() { delete this->pop_back();  }             //!< delete pop_back
        inline void delete_front() throw() { delete this->pop_front(); }             //!< delete pop_front

        inline type pull_back()  { const_type temp = **tail; delete_back();  return temp; } //!< copy back/delete/return copy
        inline type pull_front() { const_type temp = **head; delete_front(); return temp; } //!< copy front/delete/return copy


        //! syntax helper
        inline klist & operator<<(const klist &rhs)  { this->merge_back_copy(rhs); return *this; }

        //! sort list wrapper
        template <typename COMPARE_DATA>
        inline void sort(COMPARE_DATA &compare_data)
        {
            nodes_comparison<NODE,COMPARE_DATA> compare = { compare_data };
            merge_list_of<NODE>::sort(*this,compare);
        }

        //! default output
        template <typename OSTREAM> inline
        OSTREAM & display(OSTREAM &os, const char pfx='[', const char sfx=']', const char sep=';' ) const
        {
            if(pfx)
                os << pfx;
            const NODE *node = head;
            if(node)
            {
                os << **node;
                for(node=node->next;node;node=node->next)
                {
                    if(sep) os << sep;
                    os << **node;
                }
            }
            if(sfx)
                os << sfx;
            return os;
        }


    };

}

#endif

