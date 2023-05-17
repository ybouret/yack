
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/syntax/transcriber.hpp"

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
        
        virtual ~myparser() noexcept
        {
        }
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(myparser);
    };

#define JTRANS_CONNECT(UUID) YACK_JIVE_CONNECT(UUID,jTrans)
#define JTRANS_CONTROL(UUID) YACK_JIVE_CONTROL(UUID,jTrans)

    class jTrans : public jive::syntax::transcriber
    {
    public:
        explicit jTrans() :  jive::syntax::transcriber("JSON")
        {
            JTRANS_CONNECT(number);
            JTRANS_CONNECT(true);
            JTRANS_CONNECT(false);
            JTRANS_CONNECT(string);
            JTRANS_CONNECT(null);

            JTRANS_CONTROL(pair);
            JTRANS_CONTROL(heavy_array);
            JTRANS_CONTROL(heavy_object);
            JTRANS_CONTROL(empty_array);
            JTRANS_CONTROL(empty_object);

        }

        virtual ~jTrans() noexcept
        {
        }

        void on_number(const lexeme &lxm) {
            jive::syntax::translator::on_terminal(lxm);
        }

        void on_true(const lexeme &lxm) {
            jive::syntax::translator::on_terminal(lxm);
        }

        void on_false(const lexeme &lxm) {
            jive::syntax::translator::on_terminal(lxm);
        }

        void on_null(const lexeme &lxm) {
            jive::syntax::translator::on_terminal(lxm);
        }

        void on_string(const lexeme &lxm) {
            jive::syntax::translator::on_terminal(lxm);
        }

        void on_pair(const string &uuid, const size_t narg)
        {
            assert("pair"==uuid);
            assert(2==narg);
            jive::syntax::translator::on_internal(uuid,narg);
        }

        void on_heavy_array(const string &uuid, const size_t narg)
        {
            assert("heavy_array"==uuid);
            jive::syntax::translator::on_internal(uuid,narg);
        }

        void on_heavy_object(const string &uuid, const size_t narg)
        {
            assert("heavy_object"==uuid);
            jive::syntax::translator::on_internal(uuid,narg);
        }

        void on_empty_array(const string &uuid, const size_t narg)
        {
            assert("empty_array"==uuid);
            jive::syntax::translator::on_internal(uuid,narg);
        }

        void on_empty_object(const string &uuid, const size_t narg)
        {
            assert("empty_object"==uuid);
            jive::syntax::translator::on_internal(uuid,narg);
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(jTrans);
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
        auto_ptr<jive::syntax::xnode> tree = J.parse(src);
        YACK_ASSERT(tree.is_valid());
        tree->gv("tree.dot");
        ios::serializer::cfile::save("tree.bin",*tree);

        std::cerr << std::endl;
        std::cerr << "Using Translator" << std::endl;
        jive::syntax::translator xt;
        xt.walk(*tree,NULL);

        std::cerr << "Using Transcriber" << std::endl;
        jTrans xtc;
        xtc.walk(*tree,NULL);
        

    }

}
YACK_UDONE()


