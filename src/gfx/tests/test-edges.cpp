#include "yack/utest/run.hpp"
#include "yack/gfx/edges/detector.hpp"
#include "yack/utest/run.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/gfx/broker/blur.hpp"
#include "yack/color/convert.hpp"
#include "yack/gfx/filter/scharr.hpp"
#include "yack/gfx/broker/normalize.hpp"
#include "yack/gfx/broker/extrema.hpp"

using namespace yack;
using namespace graphic;

namespace
{
    template <size_t N>
    class indx2rgba
    {
    public:
        indx2rgba() noexcept : colors()
        {
            randomized::rand_ ran;
            srand(0);
            for(size_t i=1;i<N;++i)
            {
                rgba &c = colors[i];
                c.r = ran.in(100, 255);
                c.g = ran.in(100, 255);
                c.b = ran.in(100, 255);
            }
        }
        ~indx2rgba() noexcept {}

        inline void operator()(rgba &c, const size_t i) noexcept
        {
            c = colors[i%N];
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(indx2rgba);
        rgba colors[N];
    };


}

YACK_UTEST(edges)
{
    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      para = new concurrent::simd(topo);
    indx2rgba<100>              sz2c;
    blurring                    smooth(1.0);
    std::cerr << "#smooth=" << smooth.size() << std::endl;
    IMG.create< Scharr<5> >();
    const filters F = IMG.carve(Scharr<5>::uuid);


    if( argc > 1)
    {
        const string    fn  = argv[1];
        pixmap<rgba>    img = IMG.load(fn,NULL); IMG.save(img,"image.png",NULL);
        pixmap<rgba>    out(img.w,img.h);
        broker          dev(para,img);
        edges           E(img.w,img.h);

        broker_transform::apply(E.gfield,img,dev, color::convert<float,rgba>::make);
        IMG.emit(E.gfield, "source.png", "", dev, color::convert<rgba,float>::make,out);

        broker_blur::fuzz(E.origin,E.gfield,dev,smooth);
        IMG.emit(E.origin, "origin.png", "", dev, color::convert<rgba,float>::make,out);

        E.detect(dev,F);
        IMG.emit(E.origin, "thinning.png", "", dev, color::convert<rgba,float>::make,out);


        broker_normalize::apply(E.gfield,dev,E.gmax);
        IMG.emit(E.gfield, "gradient.png", "", dev, color::convert<rgba,float>::make,out);

        
        IMG.emit(E, "cleft.png", "", dev, color::convert<rgba,uint8_t>::make,out);

        broker_transform::apply(out,E.labels,dev,sz2c);
        IMG.save(out,"ridges.png", NULL);


        
    }

    

}
YACK_UDONE()

