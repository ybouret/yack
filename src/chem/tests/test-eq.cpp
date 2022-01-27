

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{

    chemical::library           lib;
    chemical::const_equilibrium eq("eq",1e-3);
    if(argc>1)
    {
        eq.load(argv[1],lib);
    }

    std::cerr << lib << std::endl;
    std::cerr << eq  << std::endl;

}
YACK_UDONE()
