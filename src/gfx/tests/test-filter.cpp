
#include "yack/gfx/broker/filter.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace graphic;


YACK_UTEST(filter)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = IMG.load(fn,NULL);
        broker       device(SIMD,img);

    }

}
YACK_UDONE()
