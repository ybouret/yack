
#include "yack/aqueous/weasel/parser.hpp"
#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            parser:: ~parser() noexcept
            {
            }

#define RX_ID "[:alpha:][:word:]*"
#define RX_KN "@[-+^.[:word:]]+"

            parser:: parser() : jive::parser("weasel")
            {
                rule::verbose = true;

                // describe a single species
                //const rule &end   = opt(term(';'));
                const rule &pp    = term('+');                                 // plus sign
                const rule &mm    = term('-');                                 // minus sign
                const rule &pc    = oom(pp);                                   // positive charge(s)
                const rule &nc    = oom(mm);                                   // negative charge(s)
                const rule &id    = term("id",RX_ID);                          // identifier
                const rule &sp    = agg("sp") << id << opt(choice(pc,nc));     // single species

                // describe a component
                const rule &cc    = act("cc") << mark('[') << sp << mark(']'); // species as component

                // describe actors
                const rule &cf    = term("cf","[1-9][0-9]*");                  // positive coefficient
                const rule &oc    = opt(cf);                                   // optional coefficient
                const rule &fa    = agg("fa") << opt(pp) << oc << cc;          // First Actor
                const rule &xa    = agg("xa") <<     pp  << oc << cc;          // eXtra Actor
                const rule &ac    = agg("ac") << fa << zom(xa);                // ACtors

                // describe components lhs/rhs
                const rule &ca    = choice(term('.'),ac);
                const rule &cm    = agg("cm") << opt( cat(ca,mark("<=>")) ) << ca;

                // describe equilibrium
                const rule &eq  =
                agg("eq") << id << mark(':') << cm;

                const rule &decl = cat( choice(eq,sp), opt( mark(';') ) );

                top( agg("weasel") << zom( decl ) );

                
                // lexical only extra
                plug( jive::lexical::cxx_comment::clid, "C++Comment");
                plug( jive::lexical::c_comments::clid,  "C_Comments");
                drop("[:blank:]+");
                endl("[:endl:]");
            }

        }

    }

}


