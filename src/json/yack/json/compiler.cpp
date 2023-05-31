#include "yack/json/compiler.hpp"
#include "yack/jive/parser.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/syntax/transcriber.hpp"

namespace yack
{

    namespace JSON
    {
        //----------------------------------------------------------------------
        //
        //
        // creating a JSON reader
        //
        //
        //----------------------------------------------------------------------
        class Compiler:: Reader : public jive::parser
        {
        public:

            //------------------------------------------------------------------
            // create parsing rules
            //------------------------------------------------------------------
            inline explicit Reader() :
            jive::parser(Compiler::call_sign)
            {
                compound   &self   = alt("JSON");
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

                validate();
            }

            //------------------------------------------------------------------
            // cleanup
            //------------------------------------------------------------------
            inline virtual ~Reader() noexcept {}


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Reader);
        };
    }

}

#include "yack/system/imported.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/ios/ascii/convert.hpp"

namespace yack
{

    namespace JSON
    {
#define JSON_CONNECT(UUID) YACK_JIVE_CONNECT(UUID,Linker)
#define JSON_CONTROL(UUID) YACK_JIVE_CONTROL(UUID,Linker)

        //----------------------------------------------------------------------
        //
        //
        // creating a JSON linker from a C++ transcriber
        //
        //
        //----------------------------------------------------------------------
        class Compiler::Linker : public jive::syntax::transcriber
        {
        public:

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            inline explicit Linker(const Compiler::Reader &r) :
            jive::syntax::transcriber(r.label,true),
            result(),
            values(),
            pairs()
            {
                // connect terminals
                JSON_CONNECT(string);
                JSON_CONNECT(null);
                JSON_CONNECT(true);
                JSON_CONNECT(false);
                JSON_CONNECT(number);

                // control internals
                JSON_CONTROL(pair);
                JSON_CONTROL(empty_array);
                JSON_CONTROL(heavy_array);

                JSON_CONTROL(empty_object);
                JSON_CONTROL(heavy_object);
            }

            inline virtual ~Linker() noexcept
            {
            }

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            Value            result; //!< final result
            solo_list<Value> values; //!< stack of values
            solo_list<Pair>  pairs;  //!< stack of pairs

            //------------------------------------------------------------------
            //
            // entry point method
            //
            //------------------------------------------------------------------
            Value &build(const jive::syntax::xnode &tree)
            {
                walk(tree,NULL);
                if(values.size!=1)
                    raise_corrupted(*label);
                pop_value(result);
                reset();
                return result;
            }

            //------------------------------------------------------------------
            //! reset status
            //------------------------------------------------------------------
            inline void reset() noexcept
            {
                values.clear();
                pairs.clear();
            }

            //------------------------------------------------------------------
            //! initialize linker
            //------------------------------------------------------------------
            inline virtual void on_init() {
                result.nil();
                reset();
            }

            //------------------------------------------------------------------
            //! create a new nil value at end of values
            //------------------------------------------------------------------
            inline void push(Value &v) {
                const Value nil;
                values << nil;
                (**values.tail).xch(v);
            }

            //------------------------------------------------------------------
            //
            // connect
            //
            //------------------------------------------------------------------
            inline void on_string(const lexeme &lxm) {
                const string s = lxm.data.to_string(1,1);
                Value        v(s);
                push(v);
            }

            inline void on_null(const lexeme &) {
                Value nil; push(nil);
            }
            

            inline void on_true(const lexeme &) {
                Value flag(true); push(flag);
            }

            inline void on_false(const lexeme &) {
                Value flag(false); push(flag);
            }

            inline void on_number(const lexeme &lxm) {
                const string s = lxm.data.to_string();
                const Number d = ios::ascii::convert::real<Number>(s,"Number");
                Value        v(d);
                push(v);
            }

            inline void raise_corrupted(const string &uuid) const {
                throw imported::exception( (*label)(), "corrupted %s", uuid());
            }

            inline void pop_value(Value &target) noexcept
            {
                assert(values.size>0);
                target.xch( **values.tail );
                values.cut_tail();
            }

            inline string pop_string() noexcept {
                const string s = (**values.tail).as<String>();
                values.cut_tail();
                return s;
            }

            //------------------------------------------------------------------
            //
            // control
            //
            //------------------------------------------------------------------
            inline void on_pair(const string &uuid, const size_t argn) {
                if(2!=argn) raise_corrupted(uuid);
                assert(values.size>=2);
                Value        data;  pop_value(data);
                const string pkey = pop_string();
                Pair         pair = new Pair_(pkey); (**pair).xch(data);
                std::cerr << "pair : " << pair << std::endl;
                pairs << pair;
            }

            void on_empty_array(const string &uuid, const size_t argn) {
                if(0!=argn) raise_corrupted(uuid);
                Value arr( asArray );
                push(arr);
            }

            void on_heavy_array(const string &uuid, const size_t argn) {
                if(argn<=0) raise_corrupted(uuid);
                assert(values.size>=argn);
                Value  val(asArray);
                {
                    Array &arr = val.as<Array>();
                    arr.reserve(argn);
                    for(size_t i=argn;i>0;--i)
                    {
                        { Value nil; arr.push_back(nil); }
                        pop_value(arr.back());
                    }
                    arr.reverse();
                }
                push(val);
            }

            void on_empty_object(const string &uuid, const size_t argn) {
                if(0!=argn) raise_corrupted(uuid);
                Value obj( asObject );
                push(obj);
            }

            void on_heavy_object(const string &uuid, const size_t argn) {
                if(argn<=0) raise_corrupted(uuid);
                assert(pairs.size>=argn);
                Value val( asObject );
                {
                    Object &obj = val.as<Object>();
                    for(size_t i=argn;i>0;--i)
                    {
                        const Pair &p = **pairs.tail;
                        if(!obj.insert( p ))
                        {
                            throw imported::exception( (*label)(), "multiple key '%s' in object", p->key_() );
                        }
                        pairs.cut_tail();
                    }
                    obj.reverse();
                }
                push(val);
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Linker);
        };
    }

}




namespace yack
{

    namespace JSON
    {

        const char  * const Compiler:: call_sign = "JSON";

        static void *reader_addr[ YACK_WORDS_FOR(Compiler::Reader) ] = { 0 };
        static void *linker_addr[ YACK_WORDS_FOR(Compiler::Linker) ] = { 0 };

        void Compiler:: cleanup() noexcept
        {
            if(linker)
            {
                out_of_reach::zset( destructed( coerce_cast<Linker>(linker_addr) ), sizeof(linker_addr) );
                linker = NULL;
            }

            if(reader) {
                out_of_reach::zset( destructed( coerce_cast<Reader>(reader_addr) ), sizeof(reader_addr) );
                reader = NULL;
            }
        }
        
        Compiler:: ~Compiler() noexcept
        {
            cleanup();
        }

        Compiler:: Compiler() :
        singleton<Compiler>(),
        reader( NULL ),
        linker( NULL )
        {
            reader = new ( YACK_STATIC_ZSET(reader_addr) ) Reader();
            try
            {
                linker  = new ( YACK_STATIC_ZSET(linker_addr) ) Linker(*reader);
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }

        Value & Compiler:: operator()(jive::module *m)
        {
            auto_ptr<jive::syntax::xnode> tree = reader->parse(m);
            return linker->build(*tree);
        }

        void Compiler:: gc() noexcept
        {
            linker->result.nil();
            linker->values.release();
            linker->pairs.release();
        }

    }

}

