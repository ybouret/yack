
#include "yack/gfx/image/format/png.hpp"
#include "yack/gfx/image/format/jpeg.hpp"
#include "yack/gfx/image/format/tiff.hpp"
#include "yack/gfx/image/format/bmp.hpp"

#include "yack/gfx/image/codecs.hpp"

namespace yack
{

    namespace graphic
    {
        namespace image
        {
            void codecs:: load_built_in()
            {
                (void) get<png_format>();
                (void) get<jpeg_format>();
                (void) get<tiff_format>();
                (void) get<bmp_format>();
            }

        }
    }

}
