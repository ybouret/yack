
#include "yack/aqueous/lua/equilibria.hpp"
#include "yack/aqueous/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(eqs)
{
    library        lib;
    lua_equilibria eqs;
    const species & h = lib("H+",  1);
    const species & w = lib("HO-",-1);

    
    {
        equilibrium &water = eqs( new const_equilibrium("water",1e-14,eqs.next_indx() ) );
        water(1,h);
        water(1,w);
        std::cerr << water << std::endl;

    }

}
YACK_UDONE()

