
#include "yack/aqueous/weasel/parser.hpp"
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
            jive::source src( jive::module::open_list("line",line) );
        }
    }


}
YACK_UDONE()

