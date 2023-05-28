
#include "yack/chem/weasel/designer.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/vat.hpp"

#include "yack/chem/eqs/lua.hpp"


using namespace yack;
using namespace Chemical;

YACK_UTEST(vat)
{

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
    Vat vat(lib,eqs,xml);

    YACK_SIZEOF(Cluster);

}
YACK_UDONE()

