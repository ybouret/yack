

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

    eqs->dostring("function F(t) return 2+math.sin(t); end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;


    vector<double> C(lib.size(),0);
    vector<double> arr(lib.size(),0);
    lib.fill(C,1,ran);
    lib(std::cerr,C);
    for(const enode *node=eqs.head();node;node=node->next)
    {
        vector<int>        nu(lib.size(),0);
        vector<double>     psi(lib.size(),0);
        const equilibrium &eq = ***node;
        eq.fill(nu);
        eqs.pad(std::cerr << "nu_" << eq.name << ' ',eq) << " = " << nu << std::endl;
        eq.drvs_action(psi,eq.K(0),C,arr);
        eqs.pad(std::cerr << "psi_" << eq.name,eq) << " = " << psi << std::endl;
    }
    

}
YACK_UDONE()

