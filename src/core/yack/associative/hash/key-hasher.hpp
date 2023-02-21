//! \file

#ifndef YACK_ASSOCIATIVE_KEY_HASHER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_HASHER_INCLUDED 1

#include "yack/hashing/to.hpp"
#include "yack/hashing/fnv.hpp"

namespace yack
{

    //______________________________________________________________________
    //
    //! key hasher based on a hashing function
    //______________________________________________________________________
    template <typename KEY,typename FUNCTION = hashing::fnv>
    class key_hasher : public hashing::to_key<size_t,FUNCTION>
    {
    public:
        typedef hashing::to_key<size_t,FUNCTION> self_type; //!< alias

        inline explicit key_hasher() noexcept : self_type() {} //!< setup
        inline virtual ~key_hasher() noexcept {}               //!< cleanup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_hasher);
    };

}


#endif

