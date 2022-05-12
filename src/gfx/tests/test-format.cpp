#include "yack/utest/run.hpp"
#include "yack/gfx/image/format/png.hpp"
#include "yack/gfx/image/format/jpeg.hpp"
#include "yack/gfx/image/format/tiff.hpp"
#include "yack/gfx/image/format/bmp.hpp"

#include "yack/gfx/image/codecs.hpp"


using namespace yack;
using namespace graphic;

YACK_UTEST(format)
{

    images &IMG = images::instance();
    std::cerr << IMG.name << " / " << IMG.call_sign << std::endl;

    {
        png_format &PNG = IMG.get<png_format>();
        PNG.extensions.scheme->makeGV("png-ext.dot");

        jpeg_format &JPEG = IMG.get<jpeg_format>();
        JPEG.extensions.scheme->makeGV("jpeg-ext.dot");

        tiff_format &TIFF = IMG.get<tiff_format>();
        TIFF.extensions.scheme->makeGV("tiff-ext.dot");

        IMG.decl(new bmp_format());
        bmp_format &BMP = IMG.get<bmp_format>();
        BMP.extensions.scheme->makeGV("bmp-ext.dot");
    }






}
YACK_UDONE()

