

#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eqs)
{
    library        lib;
    lua_equilibria eqs;

    eqs->dostring("function F(t) return 2+math.sin(t); end");

    equilibrium & water = eqs.create("water",1e-14);
    water.parse_with(lib,"[H+]+[HO-]");

    std::cerr << water << std::endl;

    if(argc>1)
    {
        eqs(lib,argv[1]);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    

}
YACK_UDONE()

