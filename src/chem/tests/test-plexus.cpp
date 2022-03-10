


#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(plexus)
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

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    plexus cs(lib,eqs);

    vector<double> C(cs.M,0);
    lib.fill(C, 1, ran);

    cs.computeK(0);

    cs.computeGamma(C);
    std::cerr << "Gamma = " << cs.Gamma << std::endl;
    cs.computePsi(C);
    std::cerr << "Psi   = " << cs.Psi << std::endl;

    cs.computeState(C);
    std::cerr << "Gamma = " << cs.Gamma << std::endl;
    std::cerr << "Psi   = " << cs.Psi << std::endl;

    cs.computeXi(C);
    std::cerr << "Xi =" << cs.Xi  << std::endl;
    std::cerr << "Ceq=" << cs.Ceq << std::endl;


}
YACK_UDONE()

