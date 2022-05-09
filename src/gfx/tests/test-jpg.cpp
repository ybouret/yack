#include "yack/gfx/format/jpeg.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/gfx/format/bmp.hpp"
#include "yack/color/rgba/from-rgba.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(jpg)
{
    options opts;
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = graphic:: jpeg_format:: load(fn);
        {
            bmp_format       bmp;
            color::from_rgba cnv;
            bmp.save("img-jpeg.bmp",img, cnv);
        }
        graphic::jpeg_format::save(img,"img-jpeg.jpg",&opts);

    }
}
YACK_UDONE()


