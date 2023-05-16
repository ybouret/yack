#include "yack/jive/dsl/parser.hpp"

#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/gz/ostream.hpp"
#include "yack/jive/syntax/translator.hpp"

using namespace yack;
using namespace jive;


namespace  yack
{
    namespace jive
    {

        class meta_parser
        {
        public:
            parser::handle self;


            explicit meta_parser(const string &uid) :
            self( new parser(uid) )
            {
            }

            virtual ~meta_parser() noexcept
            {

            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(meta_parser);
        };
    }
}


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
        auto_ptr<syntax::xnode> tree = dsl.compile( jive::module::open_file(argv[1]));
        tree->gv("dsl.dot");

        {
            ios::gz::ostream fp("dsl-tree.bin.gz");
            const size_t nw = tree->serialize(fp);
            std::cerr << "saved in binary #" << nw << std::endl;
        }

        syntax::translator tr;
        tr.walk(*tree, NULL);

    }

}
YACK_UDONE()

