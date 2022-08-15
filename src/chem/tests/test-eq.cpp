#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(eq)
{
    randomized::rand_ ran;
    library           lib;
    luaEquilibria     eqs;
    rmulops           xmul;
    raddops           xadd;

    eqs->dostring("function f(t) return 1.1+math.cos(t) end");
    
    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }
    
    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    const size_t M = lib.size();
    vector<double> C(M,0);
    vector<double> S(M,0);
    vector<double> Ctry(M,0);

    lib.fill(C,ran);

    for(const enode *node=eqs.head();node;node=node->next)
    {
        const equilibrium &eq  = ***node;
        const double       K   = eq.K(0);
        const outcome      res = outcome::study(eq, K, C, S, xmul, xadd);
        std::cerr << res << " @" << S << std::endl;
        const string  fn = "out_" + eq.name + ".dat";
        ios::ocstream fp(fn);

        const size_t NP = 100;
        for(size_t i=0;i<=NP;++i)
        {
            const double u = double(i)/NP;
            const double x = res.value * u;
            fp("%.15g %.15g %.15g\n",x, eq.mass_action(K,C,S,u,Ctry,xmul), u );
        }

    }



}
YACK_UDONE()


