
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(plexus)
{
    //concurrent::single::verbose = true;
    
    randomized::rand_ ran;

    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function K(t) return 1e-3*(1.1+math.cos(t)); end");

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    plexus cs(lib,eqs);

    cs.computeK(0);
    std::cerr << "K=" << cs.K << std::endl;

    vector<double> C(lib.size(),0);
    //lib.fill(C,0.8, ran);
    lib(std::cerr,C);
    cs.computeGamma(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    cs.computeGammaAndPsi(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Psi  =" << cs.Psi   << std::endl;
    cs.computeOmega();
    std::cerr << "Omega=" << cs.Omega << std::endl;

    cs.study();

}
YACK_UDONE()
