//! \file

#ifndef YACK_GFX_UTILS_INCLUDED
#define YACK_GFX_UTILS_INCLUDED 1

#include "yack/gfx/tessellation.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace graphic
    {
        struct command_line
        {
            static coord WxH(const string &);
            static coord WxH(const char   *);
        };
    }
}

#endif

