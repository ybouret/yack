#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/jive/syntax/rule/inner.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(syntax_types)
{
    std::cerr << "[LEXICAL]" << std::endl;
    YACK_SIZEOF(jive::token);
    YACK_SIZEOF(jive::lexeme);
    std::cerr << std::endl;

    std::cerr << "[SYNTAX:TREE]" << std::endl;
    YACK_SIZEOF(jive::syntax::xnode);
    YACK_SIZEOF(jive::syntax::xlist);
    std::cerr << std::endl;

    std::cerr << "[SYNTAX:RULE]" << std::endl;
    YACK_SIZEOF(jive::syntax::rule);
    YACK_SIZEOF(jive::syntax::terminal);
    YACK_SIZEOF(jive::syntax::internal);
    YACK_SIZEOF(jive::syntax::inner);
    std::cerr << std::endl;

    const jive::tag              name = jive::tags::make( "demo" );
    const jive::syntax::inner    demo(name);

    auto_ptr<jive::syntax::xnode> tree =  jive::syntax::xnode::make(demo);
    

}
YACK_UDONE()
