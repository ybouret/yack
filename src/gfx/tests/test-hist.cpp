
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/gfx/broker/histogram.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/color/rgba/make-gs8.hpp"

using namespace yack;
using namespace graphic;

namespace
{

    static inline
    uint8_t rgba_to_byte(const rgba &c)
    {
        return color::make_gs8::table[c.r+c.g+c.b];
    }

}

YACK_UTEST(hist)
{
    images   &IMG = images::standard();
    histogram H;
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = IMG.load(fn,NULL);
        broker       device(SIMD,img);

        broker_histogram::compute(H,img,device,rgba_to_byte);

        H.save("hist.dat");

    }

}
YACK_UDONE()

