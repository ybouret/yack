

#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: term__(const tag &id, const term_role role)
            {
                return decl( new terminal(id,role) );
            }

        }

    }
}

