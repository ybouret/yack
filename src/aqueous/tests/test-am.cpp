
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/eqs/aftermath.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(am)
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
    vector<double> C0(M+2,0);
    vector<double> Cs(M,0);
    lib.conc(C0,ran);
    for(size_t i=M;i>0;--i) if( ran.choice() ) C0[i] = 0;

    lib(std::cerr << "C0=",C0) << std::endl;

    for(enode *en=eqs->head;en;en=en->next)
    {
        equilibrium   &eq = ***en;
        const double   K  = eq.K(0);
        std::cerr << "ma=" << eq.mass_action(C0,K,xmul) << std::endl;
        aftermath am = aftermath::solve(eq,K,C0, Cs,xlim,xmul,xadd);
        
    }


}
YACK_UDONE()

