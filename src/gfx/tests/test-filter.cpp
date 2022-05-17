
#include "yack/gfx/broker/filter.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/color/rgba/make-gsf.hpp"
#include "yack/color/rgba/from-float.hpp"

#include "yack/gfx/broker/normalize.hpp"


using namespace yack;
using namespace graphic;


YACK_UTEST(filter)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);

    const float fy[3][3] = {
        {0,1,0},
        {0,0,0},
        {0,-1,0}
    };

    const float fx[3][3] = {
        {0,0,0},
        {-1,0,1},
        {0,0,0}
    };


    const filter Fy("Fy",&fy[0][0],3,3,filter::direct);
    const filter Fx("Fx",&fx[0][0],3,3,filter::direct);
    const filter Fx2("Fx2",&fy[0][0],3,3,filter::rotate);


    std::cerr << Fy << std::endl;
    std::cerr << Fx << std::endl;
    std::cerr << Fx2 << std::endl;


    if(argc>1)
    {
        const string    fn  = argv[1];
        pixmap<rgba>    img = IMG.load(fn,NULL);
        pixmap<rgba>    output(img.w,img.h);

        IMG.save(img,"img.png",NULL);
        broker          device(SIMD,img);
        color::make_gsf   rgba_to_float;
        color::from_float float_to_rgba;

        pixmap<float> source(img,device,rgba_to_float);
        pixmap<float> target(source.w,source.h);

        broker_filter::apply(target,source,device,Fy);
        broker_normalize::apply(target,device);

        broker_transform::apply(output,target,device,float_to_rgba);
        IMG.save(output, "fy.png", NULL);

        broker_filter::apply(target,source,device,Fx);
        broker_normalize::apply(target,device);

        broker_transform::apply(output,target,device,float_to_rgba);
        IMG.save(output, "fx.png", NULL);


        const filter::real_t gmax = broker_filter::gradient(target,source,device,Fx,Fy);
        std::cerr << "gmax=" << gmax << std::endl;
        broker_normalize::apply(target,device,0.0f,gmax);

        broker_transform::apply(output,target,device,float_to_rgba);
        IMG.save(output, "grad.png", NULL);
    }

}
YACK_UDONE()
