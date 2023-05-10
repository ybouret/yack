
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
        const char *csv_terminals[] = {
            "RAW",
            "STR",
            ",",
            "ENDL"
        };


        //--------------------------------
        // defines
        //--------------------------------
#define CSV_RAW      0 //!< "RAW"
#define CSV_STR      1 //!< "STR"
#define CSV_COMMA    2 //!< ","
#define CSV_ENDL     3 //!< "ENDL"

        //--------------------------------
        // keywords
        //--------------------------------
        const char *csv_internals[] = {
            "LINE"
        };


        //--------------------------------
        // defines
        //--------------------------------
#define CSV_LINE        0 //!< "LINE"


        class Parser:: Translator : public spot_object, public jive::syntax::translator
        {
        public:
            enum NodeType
            {
                NilNode,
                RawNode,
                StrNode,
                SepNode
            };

            class Node : public object
            {
            public:
                const NodeType type;
                const string  *pstr;
                Node          *next;
                Node          *prev;

                inline explicit Node(const NodeType t) noexcept : type(t), pstr(0), next(0), prev(0)
                {
                    assert(SepNode==type || NilNode==type );
                }

                inline virtual ~Node() noexcept {}
                inline explicit Node(const NodeType t, const string &s) noexcept :
                type(t), pstr( &s ), next(0), prev(0)
                {
                    assert(RawNode==type || StrNode==type);
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Node &self)
                {
                    switch(self.type)
                    {
                        case NilNode: os << "[NIL]"; assert(NULL==self.pstr); break;
                        case SepNode: os << "[SEP]"; assert(NULL==self.pstr); break;
                        case RawNode: os << "[RAW]"; assert(NULL!=self.pstr); os << '[' << *self.pstr << ']'; break;
                        case StrNode: os << "[STR]"; assert(NULL!=self.pstr); os << '[' << *self.pstr << ']'; break;
                    }
                    return os;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Node);
            };

            auto_ptr<Document>      doc;
            const hashing::perfect  termH;
            const hashing::perfect  ctrlH;
            cxx_list_of<Node>       nodes;
            solo_list<string>       texts;

            inline explicit Translator() :
            doc(NULL),
            termH(csv_terminals,sizeof(csv_terminals)/sizeof(csv_terminals[0])),
            ctrlH(csv_internals,sizeof(csv_internals)/sizeof(csv_internals[0])),
            nodes(),
            texts()
            {}

            inline virtual ~Translator() noexcept {}

            inline void clear_all() noexcept
            {
                nodes.release();
                texts.clear();
            }

            inline virtual void on_init() {
                std::cerr << "Init CSV" << std::endl;
                clear_all();
                doc = new Document();
            }

            inline virtual void on_quit() noexcept
            {
                std::cerr << "Quit CSV" << std::endl;
                std::cerr << nodes << std::endl;
                clear_all();
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
                        if(nodes.size<=0)
                        {
                            //--------------------------------------------------
                            // push Nil/Sep
                            //--------------------------------------------------
                            nodes.push_back( new Node(NilNode) );
                            nodes.push_back( new Node(SepNode) );
                        }
                        else
                        {
                            //--------------------------------------------------
                            // check previous
                            //--------------------------------------------------
                            switch(nodes.tail->type)
                            {
                                case SepNode:
                                    nodes.push_back( new Node(NilNode) );
                                    break;

                                default:
                                    break;

                            }

                            //--------------------------------------------------
                            // and push separator
                            //--------------------------------------------------
                            nodes.push_back( new Node(SepNode) );
                        }
                        break;

                    case CSV_ENDL:
                        if(nodes.size<=0 || SepNode == nodes.tail->type)
                        {
                            //--------------------------------------------------
                            // assume empty field
                            //--------------------------------------------------
                            nodes.push_back( new Node(NilNode) );
                        }
                        break;

                    default:
                        throw exception("invalid terminal '%s' in CSV",id());
                }
            }



            virtual void on_internal(const string &uuid, const size_t args)
            {
                jive::syntax::translator::on_internal(uuid,args);

                switch(ctrlH(uuid))
                {
                    case CSV_LINE:
                        buildLine();
                        break;

                    default:
                        if("LINE*" != uuid)
                            throw exception("invalid internal '%s' in CSV",uuid());
                }
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Translator);
            inline void buildLine()
            {
                removeSep();
                std::cerr << "data: " << nodes << std::endl;
                if(nodes.size<=0) throw exception("unexpected no data in CSV");
                Line *line = doc->push_back( new Line() );
                for(const Node *node=nodes.head;node;node=node->next)
                {
                    switch(node->type)
                    {
                        case SepNode: continue;
                        case NilNode: line->push_back( new Cell() ); continue;
                        case RawNode: line->push_back( new Cell(*(node->pstr), Cell::isRaw) ); continue;
                        case StrNode: line->push_back( new Cell(*(node->pstr), Cell::isStr) ); continue;
                    }
                }
                clear_all();
            }

            
            inline void removeSep() noexcept
            {
                cxx_list_of<Node> temp;
                while(nodes.size)
                {
                    Node *node = nodes.pop_front();
                    if(SepNode==node->type)
                    {
                        delete node;
                        continue;
                    }
                    temp.push_back(node);
                }
                nodes.swap_with(temp);
            }
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
            const rule &ENDL  = EOL("ENDL", "[:endl:]",true);
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
            return tr->doc.yield();
        }

    }

}
