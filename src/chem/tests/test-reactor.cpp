#include "yack/chem/reactor.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
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


    reactor cs(lib,eqs,0.0);
    std::cerr << "lib=" << cs.lib << std::endl;
    std::cerr << "eqs=" << cs.eqs << std::endl;

    if(false)
    {
        std::cerr << "K    = "    << cs.K << std::endl;
        std::cerr << "Ktot = " << cs.Ktot << std::endl;

        cs.computeK(1.0);
        std::cerr << "K    = "    << cs.K << std::endl;
        std::cerr << "Ktot = " << cs.Ktot << std::endl;
    }

    
    vector<double> C(cs.M,0);
    

    C.ld(0);
    for(size_t iter=0;iter<1;++iter)
    {
        lib.fill(C, 1, ran);
        if(cs.solve(C))
        {
            lib(std::cerr<<"Cend=",C);
        }
    }

}
YACK_UDONE()


