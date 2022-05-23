#include "yack/utest/run.hpp"
#include "yack/gfx/blob.hpp"
#include "yack/utest/run.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/concurrent/loop/simd.hpp"
#include "yack/gfx/broker/histogram.hpp"
#include "yack/gfx/broker/blur.hpp"
#include "yack/gfx/broker/filter.hpp"
#include "yack/color/convert.hpp"
#include "yack/gfx/filter/scharr.hpp"
#include "yack/gfx/broker/normalize.hpp"

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


    typedef v2d<float> vertex;

    struct task
    {
        pixmap<float>        &target;
        const pixmap<float>  &gfield;
        const pixmap<vertex> &vfield;


        static inline void make(void          *args,
                                const tiles   &part,
                                const context & ,
                                lockable      &) throw()
        {
            assert(args);
            task                 &self   = *static_cast<task *>(args);
            pixmap<float>        &target = self.target;
            const pixmap<float>  &gfield = self.gfield;
            const pixmap<vertex> &vfield = self.vfield;

            for(const tile *node=part.head();node;node=node->next)
            {
                coord                  pos = node->start;
                pixrow<float>         &tgt = target(pos.y);
                const pixrow<float>   &grd = gfield(pos.y);
                const pixrow<vertex>  &vtx = vfield(pos.y);
                for(size_t len=node->width;len>0;--len,++pos.x)
                {
                    const vertex v  = vtx(pos.x);
                    const unit_t dx = static_cast<unit_t>(floor(v.x+0.5f));
                    const unit_t dy = static_cast<unit_t>(floor(v.y+0.5f));
                    if( absolute(dx) + absolute(dy) <= 0)
                    {
                        tgt(pos.x)=0;
                    }
                    else
                    {
                        const float  g0 = grd(pos.x);
                        const float  gm = gfield[ coord(pos.x-dx,pos.y-dy) ];
                        const float  gp = gfield[ coord(pos.x+dx,pos.y+dy) ];
                        if(g0>=gm&&g0>=gp)
                        {
                            tgt(pos.x) = g0;
                        }
                        else
                        {
                            tgt(pos.x) = 0;
                        }
                    }
                }

            }

        }
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
        pixmap<rgba>    img = IMG.load(fn,NULL); IMG.save(img,"origin.png",NULL);
        pixmap<rgba>    out(img.w,img.h);
        broker          dev(para,img);

        pixmap<float>   src(img,dev,color::convert<float,rgba>::make);
        IMG.emit(src, "src.png", "level=9", dev, color::convert<rgba,float>::make,out);

        pixmap<float>   raw(img.w,img.h);
        broker_blur::fuzz(raw,src,dev,smooth);
        IMG.emit(raw, "raw.png", "level=9", dev, color::convert<rgba,float>::make,out);

        pixmap<float>        gn(img.w,img.h);
        pixmap< v2d<float> > gv(img.w,img.h);

        const float gmax = broker_filter::gradient(gn,gv,raw,dev, *F.X, *F.Y);
        std::cerr << "gmax=" << gmax << std::endl;
        broker_normalize::apply(gn,dev,gmax);
        IMG.emit(gn, "grd.png", "level=9", dev, color::convert<rgba,float>::make,out);

        pixmap<float>       lm(img.w,img.h);

        task todo = { lm, gn, gv };
        dev(task::make,&todo);

        IMG.emit(lm, "lmx.png", "level=9", dev, color::convert<rgba,float>::make,out);

    }

    

}
YACK_UDONE()

