#include "yack/jive/dsl/parser.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace jive;


YACK_UTEST(dsl_parser)
{

    jive::dsl_parser dsl;

    vector<string>   terminals;
    vector<string>   internals;
    dsl.collect_keywords(terminals,internals);

    std::cerr << "terminals=" << terminals << std::endl;
    std::cerr << "internals=" << internals << std::endl;

    {
        ios::ocstream fp(ios::cstdout);
        dsl.emit_keywords("DSL_", fp, terminals, "Terminals");
        dsl.emit_keywords("DSL_", fp, internals, "Internals");

    }

    if(argc>1)
    {
        auto_ptr<syntax::xnode> tree = dsl.parse( jive::module::open_file(argv[1]));
        YACK_CHECK(tree.is_valid());
        tree->gv("dsl.dot");
        dsl.preprocess(& *tree);
        tree->gv("dsl-pp.dot");

        {
            ios::ocstream fp("dsl-tree.bin");
            const size_t nw = tree->serialize(fp);
            std::cerr << "saved in binary #" << nw << std::endl;
        }

    }

}
YACK_UDONE()

