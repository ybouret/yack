
//! \file

#ifndef YACK_GFX_FORMAT_JPG_INCLUDED
#define YACK_GFX_FORMAT_JPG_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/gfx/format/options.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! JPEG
        //
        //______________________________________________________________________
        class jpeg_format
        {
        public:
            static pixmap<rgba> load(const string &);
            static void save(const pixmap<rgba> &img, const string &, const options *opt);

        };
        



    }

}

#endif
