
#include "yack/chem/weasel/designer.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/vat.hpp"

#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/vat/guardian.hpp"


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

    Species::Verbose = true;
    xmlog xml("[chem]",std::cerr,Species::Verbose);
    Vat   vat(xml,eqs);

    YACK_SIZEOF(Cluster);

    return 0;
    
    vector<double> C(lib->size,0);
    Library::Conc(C,ran,0.4);

    lib(std::cerr,"[",C,"]") << std::endl;

    Guardian guard;
    guard(xml,C,vat);

    std::cerr << "injected=" << guard << std::endl;



}
YACK_UDONE()

