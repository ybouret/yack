#include "yack/jive/lexical/scanner.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
namespace
{
    class my_scanner : public jive::lexical::scanner
    {
    public:
        explicit my_scanner() : jive::lexical::scanner("my_scanner")
        {
            verbose = true;
            emit("INT", "[:digit:]+");
            emit("DBL", "[:digit:]+\\.[:digit:]*");
            emit("FLT", "[:digit:]+\\.[:digit:]*f");
            drop("[:blank:]+");
            endl("[:endl:]");
            std::cerr << *table << std::endl;

        }

        virtual ~my_scanner() throw()
        {
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(my_scanner);
    };

}
YACK_UTEST(scanner)
{
    YACK_SIZEOF(jive::scatter::table);
    YACK_SIZEOF(jive::lexical::action);
    YACK_SIZEOF(jive::lexical::directive);
    YACK_SIZEOF(jive::lexical::instruction);
    YACK_SIZEOF(jive::lexical::instructions);
    YACK_SIZEOF(jive::lexical::scanner);
    YACK_SIZEOF(jive::scatter::node);
    YACK_SIZEOF(jive::scatter::slot);

    my_scanner scan;

    jive::lexemes lxm;
    jive::lexeme *lex = 0;
    if(argc>1)
    {
        {
            bool ctrl=false;
            jive::source src( jive::module::open_file(argv[1]) );
            while(NULL!=(lex=scan.probe(src,ctrl)))
            {
                YACK_ASSERT(false==ctrl);
                lxm.push_back(lex);
                std::cerr << *lex << std::endl;
            }
        }

        for(const jive::lexeme *lx=lxm.head;lx;lx=lx->next)
        {
            std::cerr << '#' << lx->indx << " : " << *lx << std::endl;
        }


    }



}
YACK_UDONE()

