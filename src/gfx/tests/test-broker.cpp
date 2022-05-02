

#include "yack/gfx/utils.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/gfx/broker.hpp"
#include "yack/utest/run.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"
#include "yack/concurrent/thread.hpp"


using namespace yack;

namespace
{
    static inline void showBroker(void                  *,
                                  const graphic::tiles &t,
                                  lockable             &sync)
    {
        YACK_LOCK(sync);
        std::cerr << "broker@" << t << std::endl;
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

    const graphic::dimensions   dims(sz.x,sz.y);
    const concurrent::topology  topo;
    graphic::engine  parEngine = new concurrent::simd(topo);
    graphic::engine  serEngine = new concurrent::mono();

    graphic::broker  par(parEngine,dims);
    graphic::broker  ser(serEngine,dims);

    std::cerr << "Serial: " << std::endl;
    ser(showBroker,NULL);

    std::cerr << "Parallel: " << std::endl;
    par(showBroker,NULL);





}
YACK_UDONE()

