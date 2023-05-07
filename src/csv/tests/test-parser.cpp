
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

    {
        ios::ocstream fp(ios::cstderr);
        jive::syntax::grammar::emit_keywords(fp,internals,"csv_internals","CSV_");
    }



    if(argc>1)
    {
        auto_ptr<CSV::Document> doc = csv( jive::module::open_file(argv[1]) );
        std::cerr << "#line=" << doc->size << std::endl;

        std::cerr << doc << std::endl;

    }



}
YACK_UDONE()
