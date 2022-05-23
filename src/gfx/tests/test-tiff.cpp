
#include "yack/gfx/image/format/tiff.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/gfx/image/format/bmp.hpp"
#include "yack/color/convert.hpp"

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
            bmp.saveBMP("img-tiff.bmp",img, color::converting<rgba,rgba>);
        }
        gfmt.save(img,"img-tiff.tif",&opts);

    }
}
YACK_UDONE()


