//! \file

#ifndef YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED 1

#include "yack/associative/suffix/key-walker.hpp"

namespace yack
{

    template <
    typename KEY,
    typename T,
    typename ASSOCIATIVE>
    class suffix_compound : public ASSOCIATIVE
    {
    public:

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_compound);
    };

}

#endif

