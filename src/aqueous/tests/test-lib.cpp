#include "yack/aqueous/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(lib)
{
    library lib;
    YACK_SIZEOF(species);
    

    const species &h = lib("H+",1);
    std::cerr << h << std::endl;
}
YACK_UDONE()

