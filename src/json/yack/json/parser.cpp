
#include "yack/json/parser.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/exception.hpp"
#include "yack/sequence/list.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/ios/ascii/convert.hpp"

namespace yack
{

    namespace JSON
    {

        static const char *ScalarKW[] =
        {
            "null",
            "true",
            "false",
            "number",
            "string"
        };

#define YACK_JSON_NULL   0
#define YACK_JSON_TRUE   1
#define YACK_JSON_FALSE  2
#define YACK_JSON_NUMBER 3
#define YACK_JSON_STRING 4


        class Translator : public jive::syntax::translator
        {
        public:
            inline  Translator() :
            jive::syntax::translator(),
            skw( YACK_HASHING_PERFECT(ScalarKW) ),
            vstack()
            {}
            inline ~Translator() throw() {}

            const hashing::perfect skw;
            list<Value>            vstack;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Translator);

            virtual void on_init()
            {
                std::cerr << "<JSON>" << std::endl;
                vstack.free();
            }

            virtual void on_quit() throw()
            {
                std::cerr << "<JSON/>" << std::endl;
            }

            virtual void on_terminal(const lexeme &lex)
            {
                jive::syntax::translator::on_terminal(lex);
                const string name = *lex.name;
                Value v;
                switch( skw(name) )
                {
                    case YACK_JSON_NULL:  break;
                    case YACK_JSON_FALSE: v=false; break;
                    case YACK_JSON_TRUE:  v=true; break;

                    case YACK_JSON_NUMBER: {
                        const string tmp = lex.data.to_string();
                        v = ios::ascii::convert::real<double>(tmp);
                    } break;

                    case YACK_JSON_STRING: {
                        const string tmp = lex.data.to_string();
                        v = tmp;
                    } break;

                    default:
                        throw exception("unknown '%s'", name() );
                }
                {
                    const Value z;
                    vstack.push_back(v);
                }
                vstack.back().xch(v);
            }


        };


        Parser:: Parser() : jive::parser("JSON"),
        tr( new Translator() )
        {
            compound &self = alt("JSON");

            compound   &VALUE  = alt("value");
            const rule &STRING = load<jive::lexical::jstring>("string");
            const rule &NUMBER = term("number","[-+]?[:digit:]+(\\.[:digit:]*)?");
            VALUE << STRING << NUMBER << term("null") << term("true") << term("false");
            const rule &COMMA = mark(',');

            {
                compound   &ARRAY = alt("array");
                {
                    const rule &LBRACK = mark('[');
                    const rule &RBRACK = mark(']');
                    ARRAY << ( agg("empty_array") << LBRACK << RBRACK );
                    ARRAY << ( agg("heavy_array") << LBRACK << VALUE << zom( cat(COMMA,VALUE) ) <<  RBRACK );
                }
                self  << ARRAY;
                VALUE << ARRAY;
            }

            {
                compound   &OBJECT = alt("object");
                {
                    const rule &LBRACE = mark('{');
                    const rule &RBRACE = mark('}');
                    OBJECT << ( agg("empty_object") << LBRACE << RBRACE );
                    {
                        const rule &PAIR = ( agg("PAIR") << STRING << mark(':') << VALUE );
                        OBJECT << ( agg("heavy_object") << LBRACE << PAIR << zom( cat(COMMA,PAIR) ) << RBRACE);
                    }
                }
                self  << OBJECT;
                VALUE << OBJECT;
            }

            drop("[:blank:]");
            endl("[:endl:]");

            gv();
            validate();

        }

        Parser:: ~Parser() throw()
        {
        }

        void Parser:: operator()(jive::module *m)
        {
            assert(m);
            reset();
            jive::source                  src(m);
            auto_ptr<jive::syntax::xnode> ast = parse(src);
            if(ast.is_empty()) throw exception("corrupted JSON parser");

            tr->walk(*ast);

        }

    }

}

