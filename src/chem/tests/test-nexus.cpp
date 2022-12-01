#include "yack/chem/nexus.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

 

YACK_UTEST(nexus)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;
    rmulops            xmul;
    raddops            xadd;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "[[ lib ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    std::cerr << "[[ eqs ]]" << std::endl;
    std::cerr << eqs << std::endl;
    std::cerr << std::endl;

    nexus        cs(lib,eqs,0.0);

    cs.lattice.graphviz("lattice.dot",cs.corelib);

}
YACK_UDONE()

