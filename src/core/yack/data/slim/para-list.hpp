//! \file

#ifndef YACK_DATA_SLIM_PARA_LIST_INCLUDED
#define YACK_DATA_SLIM_PARA_LIST_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/slim/ptr.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! list of pointers on persistent objects
    //
    //__________________________________________________________________________
    template <typename T>
    class para_list : public cxx_list_of< slim_node< slim_ptr<T> > >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                   //!< aliases
        typedef slim_ptr<T>            data_type;  //!< alias
        typedef slim_node<data_type>   node_type;  //!< alias
        typedef cxx_list_of<node_type> list_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit para_list() throw() : list_type() {}   //!< setup empty
        inline virtual ~para_list() throw() {}                 //!< cleanup
        inline para_list(const para_list &_) : list_type(_) {} //!< copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! push_back wrapper for address of argument
        para_list & operator<<(type &obj) {
            this->push_back( new node_type(&obj,transmogrify) );
            return *this;
        }

        //! push_front wrapper for address of argument
        para_list & operator>>(type &obj) {
            this->push_front( new node_type(&obj,transmogrify) );
            return *this;
        }

        //! sorting by arguments
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::replica_sort_callback<node_type,FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }

    private:
        YACK_DISABLE_ASSIGN(para_list);
    };

}

#endif

