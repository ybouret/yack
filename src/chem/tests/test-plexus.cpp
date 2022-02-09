
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


static inline void try_solve(plexus &cs, writable<double> &C)
{
    cs.lib(std::cerr,C);
    std::cerr << "Cini=" << C << std::endl;
    std::cerr << "NuT  =" << cs.NuT << std::endl;
    cs.computeGamma(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    cs.computeGammaAndPsi(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Psi  =" << cs.Psi   << std::endl;
    cs.solve(C);
}

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
    try_solve(cs,C);

    
    lib.fill(C,0.8,ran);
    try_solve(cs,C);



}
YACK_UDONE()
