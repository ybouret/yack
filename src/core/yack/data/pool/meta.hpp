//! \file

#ifndef YACK_DATA_META_POOL_INCLUDED
#define YACK_DATA_META_POOL_INCLUDED 1

#include "yack/data/pool/cxx.hpp"
#include "yack/data/list/meta-node.hpp"

namespace yack
{

    template <typename T>
    class meta_pool : public cxx_pool_of< meta_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef meta_node<T>           node_type; //!< alias
        typedef cxx_pool_of<node_type> pool_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! clenaup
        inline virtual ~meta_pool() throw() {}

        //! setup empty
        inline explicit meta_pool() throw() : pool_type() {}

        //! copy
        inline explicit meta_pool(const meta_pool &other) : pool_type(other) {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! on-the fly store new meta node with data
        inline meta_pool & operator<<( T *user ) { this->store( new node_type(user) ); return *this; }

    private:
        YACK_DISABLE_ASSIGN(meta_pool);
    };
}

#endif

