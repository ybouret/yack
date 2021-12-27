
#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/wildcard/option.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: opt(const tag &id, const rule &r)
            {
                if(!rules.owns(&r)) throw exception("[%s] does not owns <%s>",(*lang)(), (*r.name)());
                return decl( new option(id,r) );
            }

        }

    }
}

