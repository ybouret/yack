
#include "yack/aqueous/components.hpp"
#include "yack/aqueous/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(eqs)
{
    library lib;
    const species & h = lib("H+",  1);
    const species & w = lib("HO-",-1);

    {
        components water;
        water(1,h);
        water(1,w);
        std::cerr << water << std::endl;

    }
}
YACK_UDONE()

