#include "yack/gfx/format/png.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(png)
{

    if(argc>1)
    {
        const string fn = argv[1];
        graphic:: png_load(fn);
    }

}
YACK_UDONE()
