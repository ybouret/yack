
#include "yack/gfx/format/jpeg.hpp"
#include "yack/gfx/memory.hpp"

#include "yack/exception.hpp"
#include "yack/memory/embed.hpp"
#include "yack/string.hpp"

#include "yack/ios/c/readable.hpp"

#include "jpeglib.h"

namespace yack
{
    namespace graphic
    {

        pixmap<rgba> jpeg_load(const string & )
        {

            throw exception("not implemented");
        }

    }

}


