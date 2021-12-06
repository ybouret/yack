#include "yack/jive/pattern/regexp.hpp"
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        pattern * regexp:: compile(const string &expr, const dictionary *dict)
        {
            YACK_JIVE_PRINTLN("<regex '" << expr << "'>");
            rx_compiler       rxc(expr(),expr.size(),dict);
            auto_ptr<pattern> res = rxc.expression();
            if(rxc.deep>0) throw exception("%s unfinished '%s'",rxc.clid,rxc.expr);
            YACK_JIVE_PRINTLN("<regex/>");
            return res.yield();
        }
    }
}

