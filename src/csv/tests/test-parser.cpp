
#include "yack/csv/csv.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(parser)
{
    CSV::Parser csv;

    if(argc>1)
    {
        auto_ptr<jive::syntax::xnode> tree = csv.parse( jive::module::open_file(argv[1]) );
        YACK_CHECK(tree.is_valid());
        tree->gv("csv-tree.dot");
    }



}
YACK_UDONE()
