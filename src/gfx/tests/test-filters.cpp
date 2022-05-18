
#include "yack/gfx/filter/prewitt.hpp"
#include "yack/gfx/filter/sobel.hpp"
#include "yack/gfx/broker/filter.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/color/rgba/make-gsf.hpp"
#include "yack/color/rgba/from-float.hpp"
#include "yack/gfx/broker/normalize.hpp"


using namespace yack;
using namespace graphic;

static void run_filters(const images &IMG,
                        const char   *uuid,
                        broker       &device,
                        const pixmap<float> &source,
                        pixmap<float>       &target,
                        pixmap<rgba>        &output)
{
    color::from_float float_to_rgba;
    const string  root = uuid;
    const filters F    = IMG.carve(root);
    const float   gmax = broker_filter::gradient(target,source,device,*F.X,*F.Y);
    broker_normalize::apply(target,device,gmax);
    broker_transform::apply(output,target,device,float_to_rgba);
    IMG.save(output, root+".png", NULL);

}

YACK_UTEST(filters)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);

    IMG.create< Prewitt<3> >();
    IMG.create< Prewitt<5> >();
    IMG.create< Prewitt<7> >();

    IMG.create< Sobel<3> >();
    IMG.create< Sobel<5> >();
    IMG.create< Sobel<7> >();


    //std::cerr << IMG.fdb << std::endl;


    if(argc>1)
    {
        const string    fn  = argv[1];
        pixmap<rgba>    img = IMG.load(fn,NULL);
        pixmap<rgba>    output(img.w,img.h);

        IMG.save(img,"img.png",NULL);

        broker            device(SIMD,img);
        color::make_gsf   rgba_to_float;

        pixmap<float> source(img,device,rgba_to_float);
        pixmap<float> target(source.w,source.h);

        run_filters(IMG,Prewitt<3>::uuid,device,source,target,output);
        run_filters(IMG,Prewitt<5>::uuid,device,source,target,output);
        run_filters(IMG,Prewitt<7>::uuid,device,source,target,output);

        run_filters(IMG,Sobel<3>::uuid,device,source,target,output);
        run_filters(IMG,Sobel<5>::uuid,device,source,target,output);
        run_filters(IMG,Sobel<7>::uuid,device,source,target,output);

        
    }

}
YACK_UDONE()

