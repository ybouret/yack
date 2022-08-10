

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

    const size_t M = lib.size();
    vector<double> C(M,0);
    vector<double> arr(M,0);
    lib.fill(C,1,ran);
    lib(std::cerr << "C=",C);
    for(const enode *node=eqs.head();node;node=node->next)
    {
        vector<int>        nu(M,0);
        vector<double>     psi(M,0);
        const equilibrium &eq = ***node;
        eq.fill(nu);
        eqs.pad(std::cerr << "nu_" << eq.name << "  ",eq) << " = " << nu << std::endl;
        const double K   = eq.K(0);
        const double ma0 = eq.mass_action(K,C);
        eqs.pad(std::cerr << "ma_" << eq.name << "  ",eq) << " = " << ma0 << std::endl;

        eq.drvs_action(psi,K,C,arr);
        eqs.pad(std::cerr << "psi_" << eq.name << " ",eq) << " = " << psi << std::endl;

        matrix<double> H(M,M);
        eq.hessian(H,K,C,arr);
        eqs.pad(std::cerr << "hess_" << eq.name,eq) << " = " << H << std::endl;

    }
    

}
YACK_UDONE()

