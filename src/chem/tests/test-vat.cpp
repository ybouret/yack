
#include "yack/chem/weasel/designer.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/vat.hpp"

#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/vat/normalizer.hpp"
#include "yack/chem/eqs/extents.hpp"

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

    Species::Fund fund = new Species::Bank();
    Extents       xt(fund);

    for(const eNode *en=eqs->head;en;en=en->next)
    {
        const Equilibrium &eq = ***en;
        xt.build(eq,C0,TopLevel);
        eqs.display(std::cerr, eq);
        xt.display(std::cerr) << std::endl;
    }


}
YACK_UDONE()

