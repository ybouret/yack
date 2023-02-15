
#include "yack/chem/library.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/aftermath.hpp"

#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;



YACK_UTEST(am)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;
    equilibria        &eqs_ = eqs;
    
    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }
    
    std::cerr << "[[ lib ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;
    
    std::cerr << "[[ eqs ]]" << std::endl;
    std::cerr << eqs_ << std::endl;
    std::cerr << std::endl;
    
    const size_t M = lib.size();
    const size_t N = eqs.size();
    if( N <= 0) return 0;
    
    vector<double> Cini(M,0);
    vector<double> Cend(M,0);
    vector<double> Ctmp(M,0);

    rmulops xmul;
    raddops xadd;
    for(const enode *en=eqs.head();en;en=en->next)
    {
        const equilibrium &eq = ***en;
        const double       K  = eq.K(0);
        lib.cfill(Cini,ran);
        lib(std::cerr << "Cini=","",Cini);
        std::cerr << "MA: " << eq.mass_action(K,Cini,xmul) << std::endl;
        aftermath am = aftermath::guess(eq,K,Cini,Cend,xadd,xmul);
        std::cerr << am << std::endl;
        lib(std::cerr << "Cend=","",Cend);
        std::cerr << "MA: " << eq.mass_action(K,Cend,xmul) << std::endl;
        am = aftermath::guess(eq,K,Cend,Ctmp,xadd,xmul);
        std::cerr << am << std::endl;
        lib(std::cerr << "Ctmp=","",Ctmp);
        std::cerr << "MA: " << eq.mass_action(K,Ctmp,xmul) << std::endl;
        std::cerr << "RQ: " << eq.quotient(K,Ctmp,xmul)    << std::endl;
        std::cerr << "C=" << Ctmp << std::endl;
        
    }
    
 
}
YACK_UDONE()
