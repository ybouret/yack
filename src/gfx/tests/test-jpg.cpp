#include "yack/gfx/image/format/jpeg.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/gfx/image/format/bmp.hpp"
#include "yack/color/convert.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(jpg)
{
    options     opts;
    jpeg_format gfmt;

    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = gfmt.load(fn,NULL);
        {
            bmp_format       bmp;
            bmp.saveBMP("img-jpeg.bmp",img, color::converting<rgba,rgba>);
        }
        gfmt.save(img,"img-jpeg.jpg",&opts);

    }
}
YACK_UDONE()


