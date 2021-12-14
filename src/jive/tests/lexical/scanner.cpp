#include "yack/jive/lexical/scanner.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
namespace
{
    class my_scanner : public jive::lexical::scanner
    {
    public:
        explicit my_scanner() : jive::lexical::scanner("my_scanner",NULL)
        {
            make("ID","[:word:]+",this,&my_scanner::emit);
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
    jive::lexeme *lx  = 0;
    if(argc>1)
    {
        jive::source src( jive::module::open_file(argv[1]) );
        while(NULL!=(lx=scan.probe(src)))
        {
            std::cerr << *lx << std::endl;
        }
    }



}
YACK_UDONE()

