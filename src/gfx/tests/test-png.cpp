#include "yack/gfx/format/png.hpp"

#include "yack/gfx/format/bmp.hpp"
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

    if(argc>1)
    {
        const string fn  = argv[1];
        pixmap<rgba> img = graphic:: png_load(fn);
        std::cerr << "img4: " << img.w << "x" << img.h << "@" << img.d * 8 << " bpp" << std::endl;
        {
            bmp_format bmp;
            color::from_rgba cnv;
            bmp.save("img.bmp",img, cnv);
        }

        broker       device(SIMD,img);
        pixmap<rgb>  img3(img,device,rgba2rgb);
        std::cerr << "img3: " << img3.w << "x" << img3.h << "@" << img3.d * 8 << " bpp" << std::endl;
        
    }

}
YACK_UDONE()
