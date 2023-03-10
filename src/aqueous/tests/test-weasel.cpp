
#include "yack/aqueous/weasel/parser.hpp"
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/aqueous/weasel/types.hpp"
#include "yack/aqueous/weasel/designer.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(weasel)
{

    library        lib;
    lua_equilibria eqs;
    eqs.vm->dostring("Kw=1e-14");
    eqs.vm->dostring("function Kf(t) return 1+t; end");
    weasel::designer &wd = weasel::designer::instance();

    if(argc>1)
    {
        const string fn = argv[1];
        wd(lib,eqs,jive::module::open_file(fn));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    std::cerr << wd.call_sign << std::endl;

    for(size_t i=0; i< wd.ndb; ++i)
    {
        std::cerr << "\t" << wd.edb[i] << std::endl;
    }



}
YACK_UDONE()

