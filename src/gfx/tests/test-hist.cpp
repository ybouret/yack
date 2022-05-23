
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/gfx/broker/histogram.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/color/convert.hpp"

using namespace yack;
using namespace graphic;


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

        broker_histogram::compute(H,img,device,color::convert<uint8_t,rgba>::proc);

        H.save("hist.dat");
        const size_t thr = H.Otsu();
        std::cerr << "Threshold=" << thr << std::endl;

        pixmap<rgba> fg(img.w,img.h);
        broker_histogram::process(fg,img,thr,device,color::convert<uint8_t,rgba>::proc,selector<rgba>::keep,selector<rgba>::drop);
        IMG.save(fg,"fg.png",NULL);
    }

}
YACK_UDONE()

