//! \file

#ifndef YACK_GRAPHIC_PIXEL_INCLUDED
#define YACK_GRAPHIC_PIXEL_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace graphic
    {
        template <typename T> struct pixel
        {
            static T zero();
        };
    }
}

#endif

