#include "yack/gfx/format/jpeg.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/gfx/format/bmp.hpp"
#include "yack/color/rgba/from-rgba.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(jpg)
{
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = graphic:: jpeg_load(fn);
        {
            bmp_format       bmp;
            color::from_rgba cnv;
            bmp.save("img-jpeg.bmp",img, cnv);
        }

    }
}
YACK_UDONE()


