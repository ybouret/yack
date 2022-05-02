#include "yack/gfx/format/png.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(png)
{

    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = graphic:: png_load(fn);
        std::cerr << "img: " << img.w << " x " << img.h << "@" << img.d * 8 << " bpp" << std::endl;
    }

}
YACK_UDONE()
