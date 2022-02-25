
#include "yack/gfx/pixmap.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "../../core/tests/main.hpp"

using namespace yack;
using namespace graphic;

template <typename T>
static inline
void test_io( pixmap<T> &pxm, randomized::bits &ran )
{
#if 1
    const pixmap<T> &cxm = pxm;
    for(unit_t y=0;y<pxm.h;++y)
    {
        pixrow<T> &r = pxm(y);
        for(unit_t x=0;x<pxm.w;++x)
        {
            T &s = r(x);
            s = bring::get<T>(ran);
            YACK_ASSERT(s==cxm(y)(x));
        }
    }
#endif
}


YACK_UTEST(types)
{
    randomized::rand_ ran;
    {
        pixmap<string>    pxm(20,10);
        test_io(pxm,ran);
    }

    {
        pixmap<char> pxm(200,100);
        test_io(pxm,ran);
    }


    
    
}
YACK_UDONE()

