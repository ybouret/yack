
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class mylexer : public jive::lexer
    {
    public:
        explicit mylexer() : jive::lexer("lexer")
        {
        }

        virtual ~mylexer() throw()
        {
        }
        


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mylexer);
    };
}

YACK_UTEST(analyzer)
{
    YACK_SIZEOF(jive::dictionary);
    YACK_SIZEOF(jive::lexical::scanner);
    YACK_SIZEOF(jive::lexical::analyzer);

    mylexer lex;
    std::cerr << lex.label << " is ready" << std::endl;

    std::cerr << *lex << std::endl;

}
YACK_UDONE()

