
#include "yack/gfx/blob.hpp"
#include "yack/utest/run.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/gfx/broker/histogram.hpp"
#include "yack/color/convert.hpp"

using namespace yack;
using namespace graphic;

namespace
{
    template <size_t N>
    class indx2rgba
    {
    public:
        indx2rgba() throw() : colors()
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
        ~indx2rgba() throw() {}

        inline void operator()(rgba &c, const size_t i) throw()
        {
            c = colors[i%N];
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(indx2rgba);
        rgba colors[N];
    };
}

YACK_UTEST(blob)
{
   // concurrent::single::verbose = true;
    
    randomized::rand_ ran;

    vlist V;


    for(size_t i=1000;i>0;--i)
    {
        const coord tmp( ran.in(-100,100), ran.in(-100,100) );
        V.append_back(tmp);
    }

    images                     &IMG = images::standard();
    const concurrent::topology  topo;
    engine                      para = new concurrent::simd(topo);
    indx2rgba<100>              sz2c;

    if( argc > 1)
    {
        const string    fn  = argv[1];
        pixmap<rgba>    img = IMG.load(fn,NULL);
        pixmap<rgba>    out(img.w,img.h);
        pixmap<rgba>    fg(img.w,img.h);
        broker          dev(para,img);
        IMG.save(img, "origin.png", NULL);
        {
            histogram H;
            broker_histogram::compute(H,img,dev);
            broker_histogram::process(fg,img,H.Otsu(),dev);
        }
        IMG.save(fg, "fg.png", NULL);

        blob::chart bmap(fg.w,fg.h);
        blobs B;

        B.build(bmap,dev,fg,4);
        B.no_smaller_than(3,bmap);
        std::cerr << "#4=" << (*B).size << std::endl;

        broker_transform::apply(out,bmap,dev,sz2c);
        IMG.save(out,"blob4.png", NULL);
        B.build(bmap,dev,fg,8);
        B.no_smaller_than(3,bmap);

        std::cerr << "#8=" << (*B).size << std::endl;
        broker_transform::apply(out,bmap,dev,sz2c);
        IMG.save(out,"blob8.png", NULL);
        for(const blob *b=(*B).head;b;b=b->next)
        {
            std::cerr << "blob#" << b->indx << " : " << (**b).size << std::endl;
        }
    }

}
YACK_UDONE()

