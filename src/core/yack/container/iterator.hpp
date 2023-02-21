//! \file
#ifndef YACK_ITERATOR_INCLUDED
#define YACK_ITERATOR_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace iterating
    {
        //! iterating direction
        enum direction
        {
            forward, //!< forward
            reverse  //!< reverse
        };

        const char *direction_text(const direction d) noexcept; //!< textual direction
    }

}


#endif

