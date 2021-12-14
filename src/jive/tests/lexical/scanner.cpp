#include "yack/jive/lexical/scanner.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(scanner)
{
    YACK_SIZEOF(jive::scatter::table);
    YACK_SIZEOF(jive::lexical::scanner);
    YACK_SIZEOF(jive::lexical::action);
    YACK_SIZEOF(jive::lexical::directive);

}
YACK_UDONE()

