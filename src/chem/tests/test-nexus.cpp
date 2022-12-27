#include "yack/chem/claw/custodian.hpp"
#include "yack/chem/balancing.hpp"

#include "yack/chem/eqs/lua.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

 

YACK_UTEST(nexus)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "[[ lib ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    std::cerr << "[[ eqs ]]" << std::endl;
    std::cerr << eqs << std::endl;
    std::cerr << std::endl;

    nexus        cs(lib,eqs,0.0);
    const size_t M = cs.M;
    const size_t L = cs.L;
    
    cs.singles(std::cerr << "K=" ,"K_",cs.K);
    
    cxx_array<double> Kl(L);
    cs.upgrade_lattice(Kl);
    //cs.lattice(std::cerr,"K_",Kl);

    
    vector<double> C(M,0);

    for(size_t i=M;i>0;--i) {
        C[i] = ran.choice() ? lib.concentration(ran) : -lib.concentration(ran);
    }

    lib(std::cerr << "C=","",C);

    xmlog xml(nexus::clid,std::cerr,nexus::verbose);

    custodian toto(cs,xml);

    if(false)
    {
        toto(C);
        lib(std::cerr << "C=","",C);
    }
    
    balancing titi(cs,xml);
    titi(C);



    std::cerr << "topo=" << cs.topo << std::endl;
    std::cerr << "tbal=" << cs.tbal << std::endl;


    if(cs.L>0)
    {
        matrix<bool> d(cs.L,cs.L);
        
        std::cerr << "full      topology detached:" << std::endl;
        cs.build_detached(d,*cs.topo);
        cs.print_detached(d);

        std::cerr << "conserved topology detached" << std::endl;
        cs.build_detached(d,*cs.tbal);
        cs.print_detached(d);

    }
    

    YACK_SIZEOF(nexus);
    YACK_SIZEOF(cluster);
    YACK_SIZEOF(eq_team);
    YACK_SIZEOF(conservation_laws);
    YACK_SIZEOF(claw_teams);
    
}
YACK_UDONE()

