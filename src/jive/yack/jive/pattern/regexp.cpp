#include "yack/jive/pattern/regexp.hpp"
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        pattern * regexp:: compile(const string &expr, const dictionary *dict)
        {
            rx_compiler rxc(expr(),expr.size(),dict);
            if(rxc.depth>0) throw exception("%s unfinished '%s'",rxc.clid,rxc.expr);
            return NULL;
        }
    }
}

