
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
    library lib;

    if(argc>1)
    {
        const string fn = argv[1];
        jive::source src( jive::module::open_file(fn) );
        auto_ptr<weasel::xnode> tree = wp.parse(src);
        if(tree.is_valid())
        {
            weasel::linker::simplify( & *tree );
            weasel::linker::params params = { lib };
            wl.walk(*tree,&params);
            tree->gv("tree.dot");
        }

        
    }


}
YACK_UDONE()

