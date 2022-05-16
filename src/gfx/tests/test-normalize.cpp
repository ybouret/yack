
#include "yack/gfx/broker/extrema.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"
#include "yack/utest/run.hpp"
#include "yack/color/rgba/make-gsf.hpp"

using namespace yack;
using namespace graphic;

static inline void rgba_to_float(float &f, const rgba &c) throw()
{
    f = color::make_gsf::table[c.r+c.g+c.b];
}

YACK_UTEST(normalize)
{
    images &IMG = images::standard();
    const concurrent::topology  topo;
    engine  parEngine = new concurrent::simd(topo);
    engine  serEngine = new concurrent::mono();

    if(argc>1)
    {
        pixmap<rgba>  source = IMG.load(argv[1],NULL);
        broker        par(parEngine,source);
        broker        ser(serEngine,source);
        pixmap<float> imgf(source,par,rgba_to_float);

        broker_extrema::initialize<float>(par);
        broker_extrema::initialize<float>(ser);

        broker_extrema::look_up(imgf,ser);
        broker_extrema::finalize<float>(ser);

        broker_extrema::look_up(imgf,par);
        broker_extrema::finalize<float>(par);
        
    }
}
YACK_UDONE()

