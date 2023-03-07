
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

#define ID "[:alpha:][:word:]*"
            parser:: parser() : jive::parser("weasel")
            {
                rule::verbose = true;
                const rule &pp = term('+');                                 // plus sign
                const rule &mm = term('-');                                 // minus sign
                const rule &pc = oom(pp);                                   // positive charge(s)
                const rule &nc = oom(mm);                                   // negative charge(s)
                const rule &id = term("id",ID);                             // identifier
                const rule &sp = agg("sp") << id << opt(choice(pc,nc));     // single species
                const rule &cc = act("cc") << mark('[') << sp << mark(']'); // component
                const rule &pm    = choice(pp,mm);                          // \pm
                const rule &cf    = term("cf","[1-9][0-9]*");               // positive coefficient
                const rule &oc    = opt(cf);                                // optional coefficient
                const rule &first = agg("first") << opt(pm) << oc << cc;    // first actor
                const rule &extra = agg("extra") <<     pm  << oc << cc;    // extra actor(s)

                // equilibrium: id '=' first extra* '@'
                const rule &eq    =
                agg("eq") << id << mark('=')
                << first << zom(extra)
                << term('K',"@[:word:]+");
                ;

                top( act("weasel") << zom(choice(eq,sp) ) );
                
                
                // lexical only extra
                drop("[:blank:]+");
                endl("[:endl:]");
            }

        }

    }

}

