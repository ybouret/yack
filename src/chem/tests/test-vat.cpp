
#include "yack/chem/weasel/designer.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/vat.hpp"

#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/vat/normalizer.hpp"

#include "yack/system/env.hpp"

using namespace yack;
using namespace Chemical;

YACK_UTEST(vat)
{
    randomized::rand_ ran;

    Weasel::Designer & wsl = Weasel::Designer::instance();
    Library           lib;
    LuaEquilibria     eqs;

    for(int i=1;i<argc;++i)
    {
        wsl( jive::module::open_data(argv[i]), lib, eqs);
    }
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    Species::Verbose = environment::flag("VERBOSE");
    xmlog xml("[chem]",std::cerr,Species::Verbose);
    Vat   vat(xml,eqs);

    YACK_SIZEOF(Cluster);


    vector<double> C(lib->size,0);
    Library::Conc(C,ran,0.4);

    lib(std::cerr << "C0=","[",C,"]") << std::endl;

    vector<Extended::Real> C0(C,transmogrify);

    Normalizer normalizer(vat.maxClusterSize,vat.maximumSpecies);
    normalizer(xml,C0,vat);

    for(size_t i=C.size();i>0;--i)
    {
        C[i] = *C0[i];
    }
    lib(std::cerr << "C1=","[",C,"]") << std::endl;
    lib(std::cerr << "dC=","[",normalizer.custodian,"]") << std::endl;


#if 0
    if(true)
    {
        Custodian guard(vat.maximumSpecies);
        if(vat.size)
        {
            guard.restart(C0.size());
            guard.enforce(xml,C0,*vat.head);
            std::cerr << "injected=" << guard << std::endl;
            vat.head->for_each_species(std::cerr, "d[", guard, "]", TopLevel) << std::endl;
        }
    }

    std::cerr << "#maxClusterSize=" << vat.maxClusterSize << std::endl;
    
    Equalizer equalizer(vat.maxClusterSize,vat.maximumSpecies);
    if(vat.size)
    {
        equalizer.runConserved(xml,C0,*vat.head);
        equalizer.runUnbounded(xml,C0,*vat.head);
    }
#endif

}
YACK_UDONE()

