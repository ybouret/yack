#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/eqs/xlimits.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(lim)
{
    randomized::rand_ ran;
    species::verbose = true;
    library            lib;
    lua_equilibria     eqs;
    weasel::designer  &wd = weasel::designer::instance();
    cameo::mul<double> xmul;

    for(int i=1;i<argc;++i)
    {
        wd(lib,eqs,jive::module::open_data(argv[i]));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    const size_t   M = lib->size;
    vector<double> C0(M+2,0);
    lib.conc(C0,ran);
    lib(std::cerr << "C0=",C0) << std::endl;

    xlimits xlim;
    for(enode *en=eqs->head;en;en=en->next)
    {
        equilibrium   &eq = ***en;
        const double   K  = eq.K(0);
        std::cerr << "ma=" << eq.mass_action(top_level,C0,K,xmul) << std::endl;
        (void) xlim(top_level,eq,C0);
        std::cerr << eq.name << " " << xlim << std::endl;
        
    }


}
YACK_UDONE()

