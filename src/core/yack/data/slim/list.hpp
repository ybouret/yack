//! \file

#ifndef YACK_DATA_SLIM_LIST_INCLUDED
#define YACK_DATA_SLIM_LIST_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{

    template <typename T>
    class slim_list : public cxx_list_of< slim_node<T> >
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef slim_node<T>           node_type;
        typedef cxx_list_of<node_type> list_type;

        inline explicit slim_list() throw() : list_type() {}
        inline virtual ~slim_list() throw() {}
        inline slim_list(const slim_list &_) : list_type(_) {}

        template <typename U>
        slim_list & operator<<(const U &args) {
            this->push_back( new node_type(args,transmogrify) );
            return *this;
        }

        template <typename U>
        slim_list & operator>>(const U &args) {
            this->push_front( new node_type(args,transmogrify) );
            return *this;
        }

        inline type & operator[](const size_t indx) throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }

        inline const_type & operator[](const size_t indx) const throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }

        
    private:
        YACK_DISABLE_ASSIGN(slim_list);
    };

}

#endif

