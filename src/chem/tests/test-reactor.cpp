#include "yack/chem/reactor/custodian.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;



YACK_UTEST(reactor)
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

    vector<double> K;
    reactor cs(lib,eqs,K);


    
    YACK_SIZEOF(entity);
    YACK_SIZEOF(alist);
    YACK_SIZEOF(glist);
    YACK_SIZEOF(cluster);
    YACK_SIZEOF(clusters);
    YACK_SIZEOF(reactor);
    YACK_SIZEOF(sp_repo);
    YACK_SIZEOF(eq_group);
    YACK_SIZEOF(sp_group);
    YACK_SIZEOF(eq_tier);
    YACK_SIZEOF(sp_tier);
    YACK_SIZEOF(umap);
    YACK_SIZEOF(udict);
    YACK_SIZEOF(custodian);
    YACK_SIZEOF(gvector);
    YACK_SIZEOF(gvector::type);


    cs.graphViz("reactor");

    return 0;

    vector<double> C(cs.M+2);

    for(size_t i=cs.M;i>0;--i)
    {
        C[i] = lib.concentration(ran);
        if(ran.choice()) C[i] = -C[i];
    }

    custodian guard(cs);

    cs.lib(std::cerr,"",C);

    if(guard.corrected(C))
    {
        std::cerr << "corrected" << std::endl;
    }
    else
    {
        std::cerr << "was ok" << std::endl;
    }


}
YACK_UDONE()
