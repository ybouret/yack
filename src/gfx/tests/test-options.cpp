

#include "yack/gfx/image/format/options.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace graphic;


YACK_UTEST(options)
{

    graphic::options opts;

    for(int i=1;i<argc;++i)
    {
        const string opt = argv[i];
        opts << opt;
    }

    std::cerr << "options:" << opts << std::endl;

}
YACK_UDONE()

