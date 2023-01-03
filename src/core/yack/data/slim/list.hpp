//! \file

#ifndef YACK_DATA_SLIM_LIST_INCLUDED
#define YACK_DATA_SLIM_LIST_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of alive slim_node<T>
    //
    //__________________________________________________________________________
    template <typename T>
    class slim_list : public cxx_list_of< slim_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                   //!< aliases
        typedef slim_node<T>           node_type;  //!< alias
        typedef cxx_list_of<node_type> list_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_list() throw() : list_type() {}   //!< setup empty
        inline virtual ~slim_list() throw() {}                 //!< cleanup
        inline slim_list(const slim_list &_) : list_type(_) {} //!< copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! push_back wrapper
        template <typename U>
        slim_list & operator<<(const U &args) {
            this->push_back( new node_type(args,transmogrify) );
            return *this;
        }

        //! push_front wrapper
        template <typename U>
        slim_list & operator>>(const U &args) {
            this->push_front( new node_type(args,transmogrify) );
            return *this;
        }

        //! slow access
        inline type & operator[](const size_t indx) throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }

        //! slow const access
        inline const_type & operator[](const size_t indx) const throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }


        //! sort function
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            srtcb<FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }

        
    private:
        YACK_DISABLE_ASSIGN(slim_list);
        template <typename FUNC> struct srtcb
        {
            FUNC &func;
            inline int operator()(const node_type *lhs, const node_type *rhs) {
                return func( **lhs, **rhs );
            }
        };
    };

}

#endif

