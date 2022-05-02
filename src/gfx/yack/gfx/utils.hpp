//! \file

#ifndef YACK_GFX_UTILS_INCLUDED
#define YACK_GFX_UTILS_INCLUDED 1

#include "yack/gfx/tessellation.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! command line helpers
        //
        //______________________________________________________________________
        struct command_line
        {
            static coord WxH(const string &); //!< parse wxh
            static coord WxH(const char   *); //!< parse wxh wrapper
        };
    }
}

#endif

