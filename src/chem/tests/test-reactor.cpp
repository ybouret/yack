

#include "yack/chem/mix/groups.hpp"
#include "yack/chem/mix/couples.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/mix/independency.hpp"
#include "yack/ptr/auto.hpp"


#include "yack/chem/reactor.hpp"

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


    reactor        cs(lib,eqs,0.0);
    vector<double> C(cs.M,0);

    for(size_t iter=0;iter<100;++iter)
    {
        lib.fill(C, 1, ran);
        if(cs.steady(C))
        {
            lib(std::cerr<<"Cend=",C);
        }
    }


}
YACK_UDONE()


