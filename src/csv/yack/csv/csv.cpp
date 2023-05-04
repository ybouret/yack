
#include "yack/csv/csv.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/spot-object.hpp"

namespace yack
{

    namespace CSV
    {


        class Parser:: Translator : public spot_object, public jive::syntax::translator
        {
        public:
            enum NodeType
            {
                NulNode,
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

                inline explicit Node() noexcept : type(NulNode), pstr(0), next(0), prev(0) {}
                inline virtual ~Node() noexcept {}
                inline explicit Node(const NodeType t, const string &s) noexcept :
                type(t), pstr( &s ), next(0), prev(0)
                {
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Node);
            };

            cxx_list_of<Node> nodes;
            solo_list<string> texts;

            inline explicit Translator() noexcept {}
            inline virtual ~Translator() noexcept {}

            inline virtual void on_init() {
                std::cerr << "Init CSV" << std::endl;
                nodes.release();
                texts.clear();
            }

            inline virtual void on_quit() noexcept
            {
                std::cerr << "Quit CSV" << std::endl;

            }

            virtual void on_terminal(const lexeme &lx)
            {
                jive::syntax::translator::on_terminal(lx);
                std::cerr <<  "[" << lx << "]" << std::endl;
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
