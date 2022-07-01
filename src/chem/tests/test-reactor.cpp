

#include "yack/chem/mix/groups.hpp"
#include "yack/chem/mix/couples.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
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

    equilibria couples;
    equilibria lattice;

    std::cerr << lib << std::endl;
    std::cerr << "singles=" << eqs << std::endl;

    if(eqs.size() && lib.size())
    {
        library        sub(lib);
        matrix<int>    Nu(eqs.size(),lib.size());
        vector<double> K(eqs.size(),0);
        for(const enode *node=eqs.head();node;node=node->next)
        {
            const equilibrium &eq = ***node;
            const size_t       ei = *eq;
            eq.fill(Nu[ei]);
            K[ei] = eq.K(0.0);
        }

        coupling::build(couples,eqs,Nu,K,lib);
        std::cerr << "couples=" << couples << std::endl;

        lattice << eqs << couples;
        std::cerr << "lattice=" << lattice << std::endl;

    }
    groups top(lattice);
    std::cerr << top << std::endl;


}
YACK_UDONE()


