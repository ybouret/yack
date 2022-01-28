

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{

    YACK_SIZEOF(species);
    YACK_SIZEOF(equilibrium);
    
    chemical::library           lib;
    chemical::const_equilibrium eq("eq",1e-3);
    if(argc>1)
    {
        eq.load(argv[1],lib);
    }

    const size_t nv = lib.size();
    vector<double> C(nv,0);
    std::cerr << lib << std::endl;
    std::cerr << eq  << std::endl;

    lib(std::cerr,C);

    const double K0 = eq.K(0);
    std::cerr << "mass_action: " << eq.mass_action(K0,C) << std::endl;

}
YACK_UDONE()
