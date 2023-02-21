
//! \file

#ifndef YACK_DATA_HASH_NODE_INCLUDED
#define YACK_DATA_HASH_NODE_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! hash_node
        //
        //______________________________________________________________________
        template <typename KEY, typename NODE>
        class hash_node : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline virtual ~hash_node() noexcept                                      {} //!< cleanup
            inline explicit hash_node() noexcept : next(0), prev(0), hkey(0), node(0) {} //!< setup empty

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! return this freed from data
            inline hash_node *freed() noexcept
            {
                assert(NULL==prev); assert(NULL==next);
                node = 0;
                hkey = 0;
                return this;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            hash_node   *next; //!< for list/pool
            hash_node   *prev; //!< for list
            size_t       hkey; //!< hashed key
            NODE        *node; //!< user's data

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash_node);
        };
    }
    
}

#endif

