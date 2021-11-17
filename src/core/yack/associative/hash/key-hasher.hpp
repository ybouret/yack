//! \file

#ifndef YACK_ASSOCIATIVE_KEY_HASHER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_HASHER_INCLUDED 1

#include "yack/hashing/to.hpp"
#include "yack/hashing/fnv.hpp"

namespace yack
{
    template <typename KEY,typename FUNCTION = hashing::fnv>
    class key_hasher : public hashing::to_key<size_t,FUNCTION>
    {
    public:
        YACK_DECL_ARGS_(KEY,key_type);
        typedef hashing::to_key<size_t,FUNCTION> self_type;

        inline explicit key_hasher() throw() : self_type() {}
        inline virtual ~key_hasher() throw() {}
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_hasher);
    };

}


#endif

