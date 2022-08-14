#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{
    library           lib;
    luaEquilibria     eqs;
    equilibrium      &water = eqs("water",1e-14);
    water(lib,"[H+]+[HO-]");
    std::cerr << water << std::endl;


}
YACK_UDONE()


