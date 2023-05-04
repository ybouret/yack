
#include "yack/csv/csv.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(parser)
{
    CSV::Parser csv;
    vector<string> terminals;
    vector<string> internals;
    csv.collect_keywords(terminals,internals);

    std::cerr << "terminals : " << terminals << std::endl;
    std::cerr << "internals : " << internals << std::endl;

    {
        ios::ocstream fp(ios::cstderr);
        jive::syntax::grammar::emit_keywords(fp,terminals,"csv_terminals","CSV_");
    }

    if(argc>1)
    {
        csv( jive::module::open_file(argv[1]) );

#if 0
        auto_ptr<jive::syntax::xnode> tree = csv.parse( jive::module::open_file(argv[1]) );
        YACK_CHECK(tree.is_valid());
        tree->gv("csv-tree.dot");



        jive::syntax::translator tr;
        tr.walk(*tree,NULL);
#endif

    }



}
YACK_UDONE()
