

#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/compound/aggregate.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            compound &grammar:: agg_(const tag &id, const agg_role role)
            {
                return decl( new aggregate(id,role) );
            }

            static const char fn[] = "cat";
            static const char sp   = ' ';

            const rule &grammar:: cat(const rule &a, const rule &b)
            {
                must_own(a,fn);
                must_own(b,fn);
                const string r_id = '(' + *a.name + sp + *b.name + ')';
                compound    &res  = act(r_id);
                return (res << a << b);
            }

            const rule &grammar:: cat(const rule &a, const rule &b, const rule &c)
            {
                must_own(a,fn);
                must_own(b,fn);
                const string r_id = '(' + *a.name + sp + *b.name + sp + *c.name + ')';
                compound    &res  = act(r_id);
                return (res << a << b << c);
            }

        }

    }
}

