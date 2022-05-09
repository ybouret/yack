//! \file

#ifndef YACK_GFX_FORMAT_PNG_INCLUDED
#define YACK_GFX_FORMAT_PNG_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/gfx/format/options.hpp"

#include "yack/string/fwd.hpp"

namespace yack
{
    namespace graphic
    {

        class png_format
        {
        public:
            static pixmap<rgba> load(const string &, const options *);
            static void         save(const pixmap<rgba> &, const string &, const options *opt);
        };
        

    }

}

#endif
