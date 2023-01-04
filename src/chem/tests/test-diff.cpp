
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(diff)
{
    library           lib;
    luaEquilibria     eqs;
    rmulops           xmul;
    raddops           xadd;

    eqs.vm->dostring("alpha=2.1");
    const double alpha = eqs.vm->get<double>("alpha");
    eqs(lib,"@Eq:-2[A]-2[B]+[C]+3[D]:3^3/2^4*alpha^4");

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;
    std::cerr << "alpha=" << alpha << std::endl;

    const equilibrium &eq = eqs["Eq"];
    const size_t       M  = lib.size();
    const double       K  = eq.K(0);

    vector<double> C(M,0);
    vector<double> Psi(M,0);
    vector<int>    Nu(M,0);

    lib(C,"A") = 2;
    lib(C,"B") = 2;
    lib(C,"C") = 1;
    lib(C,"D") = 3;

    vector<double> S(M,0);
    vector<double> Ctry(M,0);
    eq.fill(Nu);
    const outcome  oc = outcome::study(eq, K, C, S, xmul, xadd);
    const double   Xi = (alpha-1)/(alpha+1);
    std::cerr << "outcome: " << oc << std::endl;
    std::cerr << "guess  : " <<  Xi << std::endl;
    eq.grad_action(Psi, K, S, xmul);
    const double sigma = xadd.dot(Psi,Nu);
    std::cerr << "sigma  : " << sigma << std::endl;

    {
        const size_t  NP = 1000;
        ios::ocstream fp("diff.dat");
        for(size_t i=0;i<=NP;++i)
        {
            const double u  = double(i)/NP;
            const double xi = Xi * u;
            const double g  = eq.mass_action(K,C,S,u,Ctry,xmul)/(-sigma);
            fp("%.15g %15g %.15g\n",xi,g,-(xi-Xi));
        }
    }

}
YACK_UDONE()
