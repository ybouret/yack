
#include "yack/aqueous/weasel/parser.hpp"
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/aqueous/weasel/types.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(weasel)
{
    weasel::parser wp;
    weasel::linker wl;
    wp.gv();
    library        lib;
    lua_equilibria eqs;

    eqs.vm->dostring("Kw=1e-14");
    eqs.vm->dostring("function Kf(t) return 1+t; end");

    if(argc>1)
    {
        const string fn = argv[1];
        jive::source src( jive::module::open_file(fn) );
        auto_ptr<weasel::xnode> tree = wp.parse(src);
        if(tree.is_valid())
        {
            weasel::linker::simplify( & *tree );
            weasel::linker::params params = { lib, eqs };
            wl.walk(*tree,&params);
            tree->gv("tree.dot");
        }
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

}
YACK_UDONE()

