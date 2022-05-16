
#include "yack/gfx/image/format/tiff.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/gfx/image/format/bmp.hpp"
#include "yack/color/rgba/from-rgba.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(tiff)
{
    options     opts;
    tiff_format gfmt;
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = gfmt.load(fn,NULL);
        {
            bmp_format       bmp;
            color::from_rgba cnv;
            bmp.saveBMP("img-tiff.bmp",img, cnv);
        }
        gfmt.save(img,"img-tiff.tif",&opts);

    }
}
YACK_UDONE()


