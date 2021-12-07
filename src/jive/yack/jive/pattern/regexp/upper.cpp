
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        pattern *RXCompiler:: upper()
        {
            if(curr>=last) throw exception("%s: unfinished range in '%s'",clid,expr);
            const char c = *(curr++);
            switch(c)
            {
                case '[':
                case ']':
                    throw exception("%s: unvalid '%c' for upper range in '%s'",clid,c,expr);
                    
                case '\\':
                    return bank_esc();

                default:
                    break;
            }
            return new single(c);
        }

    }

}

