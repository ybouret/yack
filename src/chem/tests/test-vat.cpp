
#include "yack/chem/weasel/designer.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/vat.hpp"

#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/vat/normalizer.hpp"
#include "yack/chem/eqs/aftermath.hpp"

#include "yack/system/env.hpp"

using namespace yack;
using namespace Chemical;

YACK_UTEST(vat)
{
    randomized::rand_ ran;

    Library           lib;
    LuaEquilibria     eqs;

    {
        Weasel::Designer & wsl = Weasel::Designer::instance();
        for(int i=1;i<argc;++i)
        {
            wsl( jive::module::open_data(argv[i]), lib, eqs);
        }
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    Species::Verbose = environment::flag("VERBOSE");
    xmlog xml("[chem]",std::cerr,Species::Verbose);
    Extended::Vector K,S;
    std::cerr << "starting with #eqs=" << eqs->size << std::endl;
    Vat              vat(xml,eqs,K);
    std::cerr << "ending   with #eqs=" << eqs->size << std::endl;

    K.make(eqs->size,0);
    S.make(eqs->size,0);
    vat.updateK(K,0.0);
    vat.displayK(std::cerr,K);
    eqs.computeScaling(S,K);
    vat.displayS(std::cerr,S);

    
    vector<double>         C(lib->size,0);
    vector<Extended::Real> C0(lib->size,0);
    vector<Extended::Real> C1(lib->size,0);
    vector<Extended::Real> Ctmp(lib->size,0);
    Normalizer             normalizer(vat.maxClusterSize,vat.maximumSpecies);

    Extended::Add xadd;
    Extended::Mul xmul;
    Species::Fund fund = new Species::Bank();
    Extents       xt(fund);

    if(false && vat.size)
    {
        for(size_t iter=0;iter<10;++iter)
        {
            C.ld(0);
            for(const Cluster *cluster=vat.head;cluster;cluster=cluster->next)
            {
                assert(cluster->Nu.rows>0);
                vector<double> extent(cluster->Nu.rows,0);
                Library::Conc(extent,ran,0.5);
                std::cerr << "extent=" << extent << std::endl;
                cluster->apply(TopLevel,C,C,extent,xadd);
            }
            lib(std::cerr << "C  = ","[",C,"]") << std::endl;
            for(size_t i=C.size();i>0;--i)
            {
                C0[i] = Extended::Send(C[i]);
            }
            normalizer(xml,C0,vat);
            for(size_t i=C.size();i>0;--i)
            {
                C[i] = *C0[i];
            }
            lib(std::cerr << "C1 = ","[",C,"]") << std::endl;
        }
    }

    for(size_t iter=0;iter<1;++iter)
    {
        Library::Conc(C,ran,0.4);
        lib(std::cerr << "C0=","[",C,"]") << std::endl;
        for(size_t i=C.size();i>0;--i)
        {
            C0[i] = Extended::Send(C[i]);
        }
        normalizer(xml,C0,vat);

        for(size_t i=C.size();i>0;--i)
        {
            C[i] = *C0[i];
        }
        lib(std::cerr << "C1=","[",C,"]") << std::endl;
        lib(std::cerr << "dC=","[",normalizer.custodian,"]") << std::endl;

        for(const eNode *en=eqs->head;en;en=en->next)
        {
            const Equilibrium   & eq = ***en;
            const size_t          ei = eq.indx[TopLevel];
            const Extended::Real  Ki = K[ei];
            const Extended::Real  Si = S[ei];
            eqs.display(std::cerr, eq) ;
            const Aftermath    am = Aftermath::Evaluate(eq,Ki,Si,C1,C0,xt,TopLevel,xmul,xadd,Ctmp);

            std::cerr << am;
            if(Equilibrium::Running==am.status) std::cerr << " Q=" << *eq.quotient(xmul,Ki,C1,TopLevel);
            std::cerr << std::endl;
            if(Equilibrium::Running==am.status)
            {
                eq.displayVar(std::cerr,C1,TopLevel);
            }
        }
    }


}
YACK_UDONE()

