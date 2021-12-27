

#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const rule &grammar:: rep(const tag &id, const rule &r, const size_t n)
            {
                if(!rules.owns(&r)) throw exception("[%s] does not owns <%s>",(*lang)(), (*r.name)());
                return decl( new repeat(id,r,n) );
            }

            const rule & grammar:: zom(const tag &id, const rule &r)
            {
                return rep(id,r,0);
            }

            const rule & grammar:: oom(const tag &id, const rule &r)
            {
                return rep(id,r,1);
            }

        }

    }
}

