//! \file
#ifndef YACK_ITERATOR_INCLUDED
#define YACK_ITERATOR_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace iterating
    {
        enum direction
        {
            forward,
            reverse
        };

        const char *direction_text(const direction d) throw();
    }

}


#endif

