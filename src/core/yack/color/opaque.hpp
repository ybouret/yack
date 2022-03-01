//! \file

#ifndef YACK_COLOR_OPAQUE_INCLUDED
#define YACK_COLOR_OPAQUE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace color
    {
        //______________________________________________________________________
        //
        //! generic opaque field
        //______________________________________________________________________
        template <typename T>
        struct opaque
        {
            static const T value; //!< 0xff or 1
        };
        
    }
}

#endif

