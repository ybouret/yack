
//! \file

#ifndef YACK_HASH_SET_INCLUDED
#define YACK_HASH_SET_INCLUDED 1

#include "yack/container/associative.hpp"

namespace yack
{

    template <typename KEY, typename T>
    class hash_set : public associative<KEY,T>
    {
    public:
        inline virtual ~hash_set() throw() {}
        inline explicit hash_set() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_set);
    };

}

#endif

