#include "yack/jive/syntax/rule.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(syntax_types)
{
    YACK_SIZEOF(jive::token);
    YACK_SIZEOF(jive::lexeme);
    YACK_SIZEOF(jive::syntax::xnode);
    YACK_SIZEOF(jive::syntax::xlist);
    YACK_SIZEOF(jive::syntax::rule);

}
YACK_UDONE()
