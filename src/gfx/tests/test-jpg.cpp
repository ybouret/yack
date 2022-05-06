#include "yack/gfx/format/jpeg.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(jpg)
{
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = graphic:: jpeg_load(fn);


    }
}
YACK_UDONE()


