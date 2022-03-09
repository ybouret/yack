
#include "yack/chem/forge.hpp"
#include "yack/chem/equilibrium.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{
    randomized::rand_ ran;

    library           lib;
    const_equilibrium water("water",1,1e-14);

    water.parse_with(lib,"[H+]+[HO-]");

    std::cerr << water << std::endl;

}
YACK_UDONE()

