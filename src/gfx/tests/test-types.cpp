
#include "yack/gfx/types.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "../../core/tests/main.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(types)
{
    randomized::rand_ ran;

    pixmap<string> pxm(10,10);

    for(unit_t y=0;y<pxm.h;++y)
    {
        pixrow<string> &r = pxm(y);
        for(unit_t x=0;x<pxm.w;++x)
        {
            string &s = r(x);
            s = bring::get<string>(ran);
        }
    }

    
    
}
YACK_UDONE()

