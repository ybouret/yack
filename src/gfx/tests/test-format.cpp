#include "yack/utest/run.hpp"
#include "yack/gfx/format/png.hpp"
#include "yack/gfx/format/jpeg.hpp"
#include "yack/gfx/format/tiff.hpp"

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

