
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

    if(argc>1)
    {
        const string    fn = argv[1];
        ios::icstream   fp(fn);
        ios::characters line;
        while((std::cout << "> ").flush(),fp.gets(line))
        {
            line.strip(" \t");
            if(line.size<=0) continue;
            jive::source                  src( jive::module::open_list("line",line) );
            auto_ptr<weasel::xnode> tree = wp.parse(src);
            if(tree.is_valid())
            {
                weasel::linker::simplify( & *tree );
                wl.walk(*tree,NULL);
                tree->gv("tree.dot");
            }
        }
    }


}
YACK_UDONE()

