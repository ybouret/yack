
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eqs)
{
    randomized::rand_ ran;

    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function K(t) return 1e-3*(1.1+math.cos(t)); end");

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;


}
YACK_UDONE()
