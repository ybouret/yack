
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
        static const char  *StructKW[] =
        {
            "empty_array",
            "heavy_array",
            "pair",
            "empty_object",
            "heavy_object"
        };

#define YACK_JSON_EMPTY_ARRAY  0
#define YACK_JSON_HEAVY_ARRAY  1
#define YACK_JSON_PAIR         2
#define YACK_JSON_EMPTY_OBJECT 3
#define YACK_JSON_HEAVY_OBJECT 4


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
            inline  Translator(Value &tgt) :
            jive::syntax::translator(),
            target(tgt),
            scalars( YACK_HASHING_PERFECT(ScalarKW) ),
            structs( YACK_HASHING_PERFECT(StructKW) ),
            vstack(),
            pstack()
            {}
            inline ~Translator() throw() {}

            Value                 &target;
            const hashing::perfect scalars;
            const hashing::perfect structs;
            list<Value>            vstack;
            list<Pair>             pstack;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Translator);

            virtual void on_init()
            {
                vstack.free();
                pstack.free();
            }

            virtual void on_quit() throw()
            {
                if(1==vstack.size())
                {
                    target.xch(vstack.back());
                }
                vstack.free();
                pstack.free();
            }

            Value & new_value()
            {
                {
                    const Value z;
                    vstack.push_back(z);
                }
                return vstack.back();
            }

            virtual void on_terminal(const lexeme &lex)
            {
                //jive::syntax::translator::on_terminal(lex);
                const string name = *lex.name;
                Value v;
                switch( scalars(name) )
                {
                    case YACK_JSON_NULL:  break;
                    case YACK_JSON_FALSE: v=false; break;
                    case YACK_JSON_TRUE:  v=true; break;

                    case YACK_JSON_NUMBER: {
                        const string tmp = lex.data.to_string();
                        v = ios::ascii::convert::real<double>(tmp);
                    } break;

                    case YACK_JSON_STRING: {
                        const string tmp = lex.data.to_string(1,1);
                        v = tmp;
                    } break;

                    default:
                        throw exception("unknown '%s'", name() );
                }
                new_value().xch(v);
            }

            virtual void on_internal(const string &name, const size_t size)
            {
                //jive::syntax::translator::on_internal(name,size);
                switch( structs(name) )
                {
                    case YACK_JSON_EMPTY_ARRAY: {
                        assert(0==size);
                        Value tmp( asArray );
                        new_value().xch(tmp);
                    } break;

                    case YACK_JSON_HEAVY_ARRAY:
                    {
                        assert(size<=vstack.size());
                        Value  tmp( asArray );
                        {
                            Array &arr = tmp.as<Array>();
                            for(size_t i=size;i>0;--i)
                            {
                                arr.push_null().xch( vstack.back() );
                                vstack.pop_back();
                            }
                            arr.reverse();
                        }
                        new_value().xch(tmp);
                    } break;

                    case YACK_JSON_PAIR: {
                        assert(2==size);
                        assert(size<=vstack.size());
                        Value         pair_val; pair_val.xch(vstack.back()); vstack.pop_back();
                        const String &pair_key = vstack.back().as<String>();
                        Pair          pair     = new Pair_(pair_key);
                        pair->val_.xch(pair_val);
                        pstack.push_back(pair);
                        vstack.pop_back();
                    } break;


                    case YACK_JSON_EMPTY_OBJECT: {
                        assert(0==size);
                        Value tmp( asObject );
                        new_value().xch(tmp);
                    } break;


                    case YACK_JSON_HEAVY_OBJECT: {
                        assert(size<=pstack.size());
                        Value  tmp( asObject );
                        {
                            Object &obj = tmp.as<Object>();
                            for(size_t i=size;i>0;--i)
                            {
                                obj.insert( pstack.back() );
                                pstack.pop_back();
                            }
                            obj.reverse();
                        }
                        new_value().xch(tmp);
                    } break;





                    default:
                        throw exception("unknown '%s'", name() );

                }

            }


        };


        Parser:: Parser() : jive::parser("JSON"),
        tr( new Translator(jv) )
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
                        const rule &PAIR = ( agg("pair") << STRING << mark(':') << VALUE );
                        OBJECT << ( agg("heavy_object") << LBRACE << PAIR << zom( cat(COMMA,PAIR) ) << RBRACE);
                    }
                }
                self  << OBJECT;
                VALUE << OBJECT;
            }

            drop("[:blank:]");
            endl("[:endl:]");

            //gv();
            validate();

        }

        Parser:: ~Parser() throw()
        {
        }

        Value &Parser:: operator()(jive::module *m)
        {
            assert(m);
            reset();
            jv.nil();

            jive::source                  src(m);
            auto_ptr<jive::syntax::xnode> ast = parse(src);
            if(ast.is_empty()) throw exception("corrupted JSON parser");

            tr->walk(*ast,NULL);


            return jv;
        }

    }

}

