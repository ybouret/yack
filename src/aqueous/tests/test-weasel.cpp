
#include "yack/aqueous/weasel/parser.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;
using namespace aqueous;

YACK_UTEST(weasel)
{
    weasel::parser wp;
    wp.gv();

    if(argc>1)
    {
        const string    fn = argv[1];
        ios::icstream   fp(fn);
        ios::characters line;
        while(fp.gets(line))
        {
            line.strip(" \t");
            if(line.size<=0) continue;
            jive::source            src( jive::module::open_list("line",line) );
            auto_ptr<weasel::xnode> tree = wp.parse(src);
            jive::syntax::translator xt;
            if(tree.is_valid())
            {
                xt.walk(*tree,NULL);
                tree->gv("tree.dot");
            }
        }
    }


}
YACK_UDONE()

