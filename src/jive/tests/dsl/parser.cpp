#include "yack/jive/dsl/parser.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace jive;


YACK_UTEST(dsl_parser)
{

    jive::dsl_parser dsl;

    if(argc>1)
    {
        auto_ptr<syntax::xnode> tree = dsl.parse( jive::module::open_file(argv[1]));
        YACK_CHECK(tree.is_valid());
        tree->gv("dsl.dot");
    }

}
YACK_UDONE()

