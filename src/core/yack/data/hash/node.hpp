
//! \file

#ifndef YACK_DATA_HASH_NODE_INCLUDED
#define YACK_DATA_HASH_NODE_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! hash_node
    //
    //__________________________________________________________________________
    template <typename KEY, typename NODE>
    class hash_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~hash_node() throw()                                      {} //!< cleanup
        inline explicit hash_node() throw() : next(0), prev(0), hkey(0), node(0) {} //!< setup empty

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! return this freed from data
        inline hash_node *freed() throw()
        {
            assert(NULL==prev); assert(NULL==next);
            node = 0;
            hkey = 0;
            return this;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        hash_node   *next; //!< for list/pool
        hash_node   *prev; //!< for list
        size_t       hkey; //!< hashed key
        NODE        *node; //!< user's data

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_node);
    };
    
}

#endif

