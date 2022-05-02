
#include "yack/gfx/utils.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(tess)
{
    graphic::coord dims(20,10);
    if(argc>1)
    {
        dims = graphic::command_line::WxH(argv[1]);
    }
    size_t cpus = 4;
    if(argc>2)
    {
        cpus = ios::ascii::convert::to<size_t>(argv[2],"cpus");
    }

    const graphic::metrics      info(dims.x,dims.y,4);
    const graphic::tessellation tess(info,cpus);

    std::cerr << tess.size() << std::endl;
    std::cerr << tess << std::endl;
}
YACK_UDONE()

