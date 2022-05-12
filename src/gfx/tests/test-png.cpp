#include "yack/gfx/image/format/png.hpp"

#include "yack/gfx/image/format/bmp.hpp"
#include "yack/color/rgba/from-rgba.hpp"


#include "yack/concurrent/loop/simd.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace graphic;

namespace
{
    static inline void rgba2rgb(rgb &out, const rgba &c) throw()
    {
        out = c;
    }
}

YACK_UTEST(png)
{
    const concurrent::topology  topo;
    engine                      SIMD = new concurrent::simd(topo);
    png_format                  gfmt;
    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = gfmt.load(fn,NULL);
        std::cerr << "img4: " << img.w << "x" << img.h << "@" << img.d * 8 << " bpp" << std::endl;
        {
            bmp_format bmp;
            color::from_rgba cnv;
            bmp.save("img-png.bmp",img, cnv);
        }
        gfmt.save(img,"img-png3.png",NULL);
        
        options opts;
        opts << "alpha=1";
        if(argc>2)
            opts << argv[2];
        gfmt.save(img,"img-png4.png",&opts);


        broker       device(SIMD,img);
        pixmap<rgb>  img3(img,device,rgba2rgb);
        std::cerr << "img3: " << img3.w << "x" << img3.h << "@" << img3.d * 8 << " bpp" << std::endl;
        
    }

}
YACK_UDONE()
