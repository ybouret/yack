#include "yack/utest/run.hpp"
#include "yack/gfx/image/format/png.hpp"
#include "yack/gfx/image/format/jpeg.hpp"
#include "yack/gfx/image/format/tiff.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(format)
{

    png_format PNG;
    PNG.scheme->makeGV("png-ext.dot");

    jpeg_format JPEG;
    JPEG.scheme->makeGV("jpeg-ext.dot");

    tiff_format TIFF;
    TIFF.scheme->makeGV("tiff-ext.dot");
}
YACK_UDONE()

