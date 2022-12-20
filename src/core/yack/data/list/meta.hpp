//! \file

#ifndef YACK_DATA_META_LIST_INCLUDED
#define YACK_DATA_META_LIST_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/data/list/meta-node.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of meta nodes
    //
    //__________________________________________________________________________
    template <typename T>
    class meta_list : public cxx_list_of< meta_node<T> >
    {
    public:
        YACK_DECL_ARGS_(T,type);

        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef meta_node<T>           node_type; //!< alias
        typedef cxx_list_of<node_type> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! clenaup
        inline virtual ~meta_list() throw() {}

        //! setup empty
        inline explicit meta_list() throw() : list_type() {}

        //! copy
        inline explicit meta_list(const meta_list &other) : list_type(other) {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! on-the fly push back new meta node with data
        inline meta_list & operator<<( T *user ) { (void) this->push_back( new node_type(user) ); return *this; }

        //! check if ref was inserted
        inline bool met(const_type &ref) const throw() {
            for(const node_type *node=this->head;node;node=node->next) {
                const_type &usr = **node;
                if( &usr == &ref ) return true;
            }
            return false;
        }

    private:
        YACK_DISABLE_ASSIGN(meta_list);
    };

   

   


}

#endif

