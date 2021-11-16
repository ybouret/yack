
//! \file

#ifndef YACK_DATA_HASH_NODE_INCLUDED
#define YACK_DATA_HASH_NODE_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    template <typename KEY, typename NODE>
    class hash_node : public object
    {
    public:
        inline virtual ~hash_node() throw() {}
        inline explicit hash_node() throw() : next(0), prev(0), hkey(0), node() {}

        hash_node   *next;
        hash_node   *prev;
        const size_t hkey;
        NODE        *node;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_node);
    };
    
}

#endif

