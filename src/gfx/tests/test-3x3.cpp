#include "yack/gfx/broker/3x3.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/color/rgba/make-gsf.hpp"
#include "yack/gfx/pixel.hpp"

using namespace yack;
using namespace graphic;



YACK_UTEST(3x3)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);


    if(argc>1)
    {
        const string    fn     = argv[1];
        pixmap<rgba>    origin = IMG.load(fn,NULL);
        pixmap<rgba>    output(origin.w,origin.h);

        IMG.save(origin,"origin.png",NULL);

        broker            device(SIMD,origin);
        color::make_gsf   rgba_to_float;

        pixmap<float> source(origin,device,rgba_to_float);
        pixmap<float> target(source.w,source.h);

        broker3x3::apply(output,origin,device, pixel<rgba>::average3x3);
        IMG.save(output,"average.png",NULL);

        broker3x3::apply(output,origin,device, pixel<rgba>::minimum3x3);
        IMG.save(output,"minimum.png",NULL);

        broker3x3::apply(output,origin,device, pixel<rgba>::maximum3x3);
        IMG.save(output,"maxium.png",NULL);


    }
}
YACK_UDONE()

