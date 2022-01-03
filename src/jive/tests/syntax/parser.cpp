
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/syntax/xnode.hpp"

using namespace yack;

namespace
{
    class myparser : public jive::parser
    {
    public:
        explicit myparser() : jive::parser("JSON")
        {
            compound   &json  = alt("JSON");

            compound   &value = alt("value");
            const rule &str   = load<jive::lexical::jstring>("string");
            value << str;
            value << term("number","[-+]?[:digit:]+");
            value << term("false") << term("true") << term("null");

            const rule &comma = mark(',');
            {
                compound &array = alt("array");
                value << array;
                const rule &lbrack = mark('[');
                const rule &rbrack = mark(']');
                array << ( agg("empty_array") << lbrack << rbrack );
                array << ( agg("heavy_array") << lbrack << value << zom( cat(comma,value) ) << rbrack);
                json << array;
            }

            {
                compound &obj = alt("object");
                value << obj;
                const rule &lbrace = mark('{');
                const rule &rbrace = mark('}');
                obj << ( agg("empty_object") << lbrace << rbrace );

                const rule &pair = (agg("pair") << str << mark(':') << value);
                obj << ( agg("heavy_object") << lbrace << pair << zom( cat(comma,pair) ) << rbrace );

                json << obj;
            }



            drop("[:blank:]+");
            endl("[:endl:]");

            gv();
            validate();
        }
        
        virtual ~myparser() throw()
        {
        }
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(myparser);
    };
}

#include "yack/ios/serializer/cfile.hpp"

YACK_UTEST(parser)
{
    jive::syntax::rule::verbose = true;
    myparser J;
    std::cerr << J.label << std::endl;

    if(argc>1)
    {
        jive::source                  src( jive::module::open_file(argv[1]) );
        auto_ptr<jive::syntax::xnode> tree = J(src);
        YACK_ASSERT(tree.is_valid());
        tree->gv("tree.dot");
        ios::serializer::cfile::save("tree.bin",*tree);

    }

}
YACK_UDONE()


