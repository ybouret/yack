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
    const size_t M = cs.M;

    vector<double> C(M,0);

    for(size_t i=M;i>0;--i) {
        C[i] = ran.choice() ? lib.concentration(ran) : -lib.concentration(ran);
    }

    lib(std::cerr,"",C);

    cs.preserve(C);




    
    

}
YACK_UDONE()

