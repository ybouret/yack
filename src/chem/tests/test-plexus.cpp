
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(plexus)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_ ran;
    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function F(t) return 2+math.sin(t); end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }


    plexus cs(lib,eqs,0.0);
    

}
YACK_UDONE()


