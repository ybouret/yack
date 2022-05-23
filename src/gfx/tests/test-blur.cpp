
#include "yack/gfx/broker/blur.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/concurrent/loop/mono.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(blur)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);

    float sigma = 1.0f;
    if( argc > 1)
    {
        sigma = ios::ascii::convert::real<float>(argv[1],"sigma");
    }
    blurring B(sigma);
    std::cerr << "sigma=" << sigma << " => radius=" << B.radius << ", #coef=" << B.size() << std::endl;
    std::cerr << B << std::endl;

    if( argc > 2)
    {
        const string    fn  = argv[2];
        pixmap<rgba>    img = IMG.load(fn,NULL);
        pixmap<rgba>    output(img.w,img.h);
        broker          device(SIMD,img);
        broker_blur::fuzz(output,img,device,B);

        IMG.save(img,"img.png",NULL);
        IMG.save(output,"blur.png",NULL);


    }

}
YACK_UDONE()

