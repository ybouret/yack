#include "yack/chem/eqs/constant.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{
    library           lib;
    const_equilibrium water("water",1,1e-14);
    water(lib,"[H+]+[HO-]");
    std::cerr << water << std::endl;
    
}
YACK_UDONE()


