#include "yack/gfx/utils.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/utest/run.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"
#include "yack/concurrent/thread.hpp"
#include "yack/color/rgba/make-gsf.hpp"


using namespace yack;
using namespace graphic;

namespace
{
    static inline void showBroker(void                  *,
                                  const graphic::tiles &t,
                                  const context        &site,
                                  lockable             &sync)
    {
        YACK_LOCK(sync);
        std::cerr << "broker" << site << " @" << t << std::endl;
    }

    static inline float rgba_to_float(const rgba &c) throw()
    {
        return color::make_gsf::table[c.r+c.g+c.b];
    }
}

YACK_UTEST(broker)
{
    graphic::coord sz(20,10);
    if(argc>1)
    {
        sz = graphic::command_line::WxH(argv[1]);
    }

    concurrent::thread::verbose = true;

    const  dimensions           dims(sz.x,sz.y);
    const concurrent::topology  topo;
    engine  parEngine = new concurrent::simd(topo);
    engine  serEngine = new concurrent::mono();

    broker  par(parEngine,dims);
    broker  ser(serEngine,dims);

    std::cerr << "Serial: " << std::endl;
    ser(showBroker,NULL);

    std::cerr << "Parallel: " << std::endl;
    par(showBroker,NULL);

    pixmap<rgba>  source(dims.w,dims.h);
    pixmap<float> target(source,par,rgba_to_float);


}
YACK_UDONE()

