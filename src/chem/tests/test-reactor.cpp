#include "yack/chem/reactor/conservation/custodian.hpp"
#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/chem/reactor/solver/steady.hpp"
#include "yack/math/iota.hpp"
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
    equilibria        &eqs_ = eqs;

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
    
    K.make(cs.L,0);
    



    //cs.graphViz("reactor");


    vector<double> C(cs.M+2);

    for(size_t i=cs.M;i>0;--i)
    {
        C[i] = lib.concentration(ran);
        if(ran.choice()) C[i] = -C[i];
    }

    lib(std::cerr << "Cini=","",C);

    if(false)
    {
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

    {
        const xmlog xml("[equalizer]",std::cerr,reactor::verbose);
        equalizer eqz(cs);
        eqz.comply(C,xml);
        lib(std::cerr << "Cend=","",C);

        cxx_array<double> xi(cs.N,0);
        for(size_t i=xi.size();i>0;--i)
        {
            xi[i] = lib.concentration(ran) * ran.symm<double>();
        }

        eqs_(std::cerr << "xi=","",xi);
        math::iota::mul(C,cs.NuT,xi,eqz.xadd);
        lib(std::cerr << "Cxi=","",C);
        eqz.comply(C,xml);
        lib(std::cerr << "Cbal=","",C);

    }

    {
        const xmlog xml("[steady]",std::cerr,reactor::verbose);
        lib.cfill(C,ran);
        cs.computeK(K,0.0);
        steady zs(cs,K);
        lib(std::cerr    << "C0=","",C);
        cs.all(std::cerr << "K=","K_",K);

        zs.run(C,xml);


    }

    if(false)
    {
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
        YACK_SIZEOF(ledger);
        YACK_SIZEOF(ledger::type);
        YACK_SIZEOF(frontier);
        YACK_SIZEOF(frontiers);
        YACK_SIZEOF(equalizer);
    }
}
YACK_UDONE()
