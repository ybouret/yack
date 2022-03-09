

#include "yack/chem/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eqs)
{
    library    lib;
    equilibria eqs;

    equilibrium & water = eqs("water",1e-14);
    water.parse_with(lib,"[H+]+[HO-]");

    std::cerr << water << std::endl;

}
YACK_UDONE()

