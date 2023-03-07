
#include "yack/aqueous/weasel/parser.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            parser:: ~parser() noexcept
            {
            }

            parser:: parser() : jive::parser("weasel")
            {
                rule::verbose = true;
                const rule &pp = term('+');
                const rule &mm = term('-');

                const rule &id = term("id","[:alpha:][:word:]*");
                const rule &pc = oom(pp);
                const rule &nc = oom(mm);
                const rule &sp = agg("sp") << id << opt(choice(pc,nc));

                const rule &cc = act("cc") << mark('[') << sp << mark(']');


                const rule &pm    = choice(pp,mm);
                const rule &cf    = term("cf","[1-9][0-9]*");
                const rule &cf_   = opt(cf);
                const rule &first = agg("first") << opt(pm) << cf_ << cc;
                const rule &extra = agg("extra") <<     pm  << cf_ << cc;
                const rule &eq    = agg("eq") << first << zom(extra);

                top( act("weasel") << zom(choice(eq,sp) ) );
                
                
                // lexical only extra
                drop("[:blank:]+");
                endl("[:endl:]");
            }

        }

    }

}


