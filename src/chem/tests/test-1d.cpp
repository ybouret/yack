

#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/eqs/aftermath.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/math/iota.hpp"

#include "yack/counting/part.hpp"

using namespace yack;
using namespace chemical;
using namespace math;



YACK_UTEST(1d)
{

    randomized::rand_ ran;
    species::verbose = true;



    library            lib;
    lua_equilibria     eqs;
    weasel::designer  &wd = weasel::designer::instance();
    cameo::mul<double> xmul;
    cameo::add<double> xadd;
    xlimits            xlim;

    for(int i=1;i<argc;++i)
    {
        wd(lib,eqs,jive::module::open_data(argv[i]));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    const size_t   M = lib->size;
    vector<double> C0(M,0);
    vector<double> Cs(M,0);
    vector<double> psi(M,0);
    vector<int>    nu(M,0);
    vector<double> Ctmp(M,0);

    lib.conc(C0,ran);
    //for(size_t i=M;i>0;--i) if( ran.choice() ) C0[i] = 0;

    lib(std::cerr << "C0=",C0) << std::endl;


    for(enode *en=eqs->head;en;en=en->next)
    {
        equilibrium   &eq = ***en;
        const double   K  = eq.K(0);

        eq.fill(top_level,nu);
        std::cerr << "nu=" << nu << std::endl;
        std::cerr << "ma=" << eq.mass_action(top_level,C0,K,xmul) << std::endl;
        const aftermath am = aftermath::solve(top_level,eq,K,C0,Cs,xlim,xmul,xadd);
        std::cerr << "am=" << am << std::endl;
        YACK_CHECK(is_running==am.state);
        lib(std::cerr << "Cs=",Cs) << std::endl;
        std::cerr << "ma0=" << eq.mass_action(top_level,Cs,K,xmul) << std::endl;

        const double  xi0 = am.value;
        const double  sig = eq.slope(top_level,Cs,K,xmul,xadd);
        std::cerr << "sig=" << sig << std::endl;


        const string  fn = eq.name + ".dat";
        ios::ocstream fp(fn);
        const size_t np = 100;
        for(size_t i=0;i<=np;++i)
        {
            const double u  = double(i)/np;
            const double xi = (i*xi0)/np;
            const double G  = eq.mass_action(top_level, u, C0, Cs, K, Ctmp, xmul);
            const double g = (xi-xi0) * sig;
            fp("%.15g %.15g %.15g %.15g\n",xi,G,g,G-g);
        }

        //std::cerr << "plot '" << fn << "' w l, 0, ";
        //std::cerr << "(x-(" << xi0 << "))*(" << sig << ")" << std::endl;


    }



}
YACK_UDONE()

