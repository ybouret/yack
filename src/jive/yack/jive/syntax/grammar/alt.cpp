


#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/compound/alternate.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            compound &grammar:: alt_(const tag &id)
            {
                return decl( new alternate(id) );
            }

            static const char fn[] = "choice";

            const rule &grammar:: choice(const rule &a, const rule &b)
            {
                must_own(a,fn);
                must_own(b,fn);
                const string r_id = '(' + *a.name + '|' + *b.name + ')';
                compound    &res  = alt(r_id);
                return (res << a << b);
            }

            const rule &grammar:: cat(const rule &a, const rule &b, const rule &c)
            {
                must_own(a,fn);
                must_own(b,fn);
                const string r_id = '(' + *a.name + '|' + *b.name + '|' + *c.name + ')';
                compound    &res  = alt(r_id);
                return (res << a << b << c);
            }

        }

    }
}

