#include "yack/aqueous/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(lib)
{
    library lib;
    YACK_SIZEOF(species);
    

    const species &h = lib("H+",1);
    const species &w = lib("HO-",-1);
    std::cerr << h << std::endl;
    std::cerr << w << std::endl;
    std::cerr << "#species=" << lib->size << std::endl;
    std::cerr << lib << std::endl;
}
YACK_UDONE()

