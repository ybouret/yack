#include "yack/gfx/image/format/tiff-stack.hpp"
#include "yack/gfx/image/formats.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(pixmaps)
{
    images &IMG = images::standard();
    if(argc>1)
    {
        const string filename = argv[1];
        tiff_stack   pxms(filename);
        std::cerr << "#pxms=" << pxms.size() << " " << pxms.w << "x" << pxms.h << std::endl;
        for(size_t i=1;i<=pxms.size();++i)
        {
            const string savename = vformat("stack%03u.png",unsigned(i));
            IMG.save(pxms[i], savename, NULL);
        }
    }
    
}
YACK_UDONE()

