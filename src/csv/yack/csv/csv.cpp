
#include "yack/csv/csv.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/spot-object.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace CSV
    {
        //--------------------------------
        // keywords
        //--------------------------------
        static const char *csv_terminals[] = {
            "RAW",
            "STR",
            ","
        };

        //--------------------------------
        // defines
        //--------------------------------
#define CSV_RAW      0 //!< "RAW"
#define CSV_STR      1 //!< "STR"
#define CSV_COMMA    2 //!< ","


        class Parser:: Translator : public spot_object, public jive::syntax::translator
        {
        public:
            enum NodeType
            {
                NilNode,
                RawNode,
                StrNode
            };

            class Node : public object
            {
            public:
                const NodeType type;
                const string  *pstr;
                Node          *next;
                Node          *prev;

                inline explicit Node() noexcept : type(NilNode), pstr(0), next(0), prev(0) {}
                inline virtual ~Node() noexcept {}
                inline explicit Node(const NodeType t, const string &s) noexcept :
                type(t), pstr( &s ), next(0), prev(0)
                {
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Node &self)
                {
                    switch(self.type)
                    {
                        case NilNode: os << "[NIL]"; assert(NULL==self.pstr); break;
                        case RawNode: os << "[RAW]"; assert(NULL!=self.pstr); os << '[' << *self.pstr << ']'; break;
                        case StrNode: os << "[STR]"; assert(NULL!=self.pstr); os << '[' << *self.pstr << ']'; break;
                    }
                    return os;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Node);
            };

            const hashing::perfect  termH;
            cxx_list_of<Node>       nodes;
            solo_list<string>       texts;

            inline explicit Translator() :
            termH(csv_terminals,sizeof(csv_terminals)/sizeof(csv_terminals[0])),
            nodes(),
            texts()
            {}

            inline virtual ~Translator() noexcept {}

            inline virtual void on_init() {
                std::cerr << "Init CSV" << std::endl;
                nodes.release();
                texts.clear();
            }

            inline virtual void on_quit() noexcept
            {
                std::cerr << "Quit CSV" << std::endl;
                std::cerr << nodes << std::endl;
            }

            const string & stringified(const lexeme &lx)
            {
                {
                    const string str = lx.data.to_string();
                    texts.annex(str);
                }
                return **texts.tail;
            }
            virtual void on_terminal(const lexeme &lx)
            {
                jive::syntax::translator::on_terminal(lx);
                std::cerr <<  "[" << lx << "]" << std::endl;
                const string &id = *lx.name;
                switch( termH(id) )
                {
                    case CSV_RAW:
                        nodes.push_back( new Node(RawNode, stringified(lx) ) );
                        break;

                    case CSV_STR:
                        nodes.push_back( new Node(StrNode, stringified(lx) ) );
                        break;

                    case CSV_COMMA:
                        break;

                    default:
                        throw exception("invalid terminal '%s' in CSV",id());
                }
            }

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Translator);
        };

        Parser:: ~Parser() noexcept
        {
            assert(tr);
            delete tr;
            tr = 0;
        }


        Parser:: Parser() : jive::parser("CSV") ,
        tr( new Translator() )
        {

            std::cerr << "sizeof(Translator)=" << sizeof(Translator) << std::endl;
            const rule &RAW   = term("RAW","[[:word:]']+");
            const rule &STR   = load<jive::lexical::jstring>("STR");
            const rule &VALUE = choice(STR,RAW);
            const rule &COMMA = term(',');
            const rule &ENDL  = EOL("ENDL", "[:endl:]",false);
            const rule &LINE  = agg("LINE") << zom( choice(VALUE,COMMA) ) << ENDL;

            top( zom(LINE) );
            drop("[:blank:]");
            validate();
            gv();
        }

        Document * Parser:: operator()(jive::module *m)
        {
            auto_ptr<jive::syntax::xnode> tree = parse(m);
            assert(tree.is_valid());
            tr->walk(*tree, NULL);
            return NULL;
        }

    }

}
