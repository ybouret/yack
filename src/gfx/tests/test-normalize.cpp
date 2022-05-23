
#include "yack/gfx/broker/extrema.hpp"
#include "yack/gfx/broker/normalize.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"
#include "yack/utest/run.hpp"
#include "yack/utest/tmx.hpp"
#include "yack/color/convert.hpp"

using namespace yack;
using namespace graphic;




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
        pixmap<float> imgf(source,par,color::convert<float,rgba>::make);


        
        broker_extrema::search(imgf,ser);
        broker_extrema::search(imgf,par);

        double duration    = 0.5;
        double look_up_ser = 0;
        YACK_TMX_RATE(look_up_ser, broker_extrema::search(imgf,ser), duration);
        std::cerr << "look_up_ser=" << look_up_ser << std::endl;
        double look_up_par = 0;
        YACK_TMX_RATE(look_up_par, broker_extrema::search(imgf,par), duration);
        std::cerr << "look_up_par=" << look_up_par << std::endl;

        const float * const res = broker_extrema::search(imgf,par);
        broker_normalize::apply(imgf,ser,res[0],res[1]);
        
        pixmap<rgba> target(imgf,par,color::convert<rgba,float>::make);
        IMG.save(target,"normalized.png",NULL);

    }
}
YACK_UDONE()

