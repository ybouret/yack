//! \file

#ifndef YACK_COLOR_OPAQUE_INCLUDED
#define YACK_COLOR_OPAQUE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace color
    {
        template <typename T>
        struct opaque
        {
            static const T value;
        };
        
    }
}

#endif

