
#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/wildcard/option.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: opt_(const tag &id, const rule &r)
            {
                if(!rules.owns(&r)) throw exception("[%s] does not owns <%s> for optional rule",(*lang)(), (*r.name)());
                return decl( new option(id,r) );
            }

            const rule & grammar:: opt(const rule &r)
            {
                const string r_id = *r.name + '?';
                const tag    id   = tags::make(r_id);
                return opt_(id,r);
            }

        }

    }
}

