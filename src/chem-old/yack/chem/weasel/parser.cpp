
#include "yack/chem/weasel/parser.hpp"
#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"
#include "yack/jive/lexical/plugin/rstring.hpp"
#include "yack/jive/lexical/plugin/bstring.hpp"

namespace yack
{
    namespace chemical
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
                //rule::verbose = true;

                //--------------------------------------------------------------
                // describe a single species
                //--------------------------------------------------------------
                const rule &pp    = term('+');                                 // plus sign
                const rule &mm    = term('-');                                 // minus sign
                const rule &pc    = oom(pp);                                   // positive charge(s)
                const rule &nc    = oom(mm);                                   // negative charge(s)
                const rule &id    = term("id",RX_ID);                          // identifier
                const rule &sp    = agg("sp") << id << opt(choice(pc,nc));     // single species

                //--------------------------------------------------------------
                // describe a component
                //--------------------------------------------------------------
                const rule &cc    = grp("cc") << mark('[') << sp << mark(']'); // species as component

                //--------------------------------------------------------------
                // describe actors
                //--------------------------------------------------------------
                const rule &cf    = term("cf","[1-9][0-9]*");                  // positive coefficient
                const rule &oc    = opt(cf);                                   // optional coefficient
                const rule &fa    = agg("fa") << opt(pp) << oc << cc;          // First Actor
                const rule &xa    = agg("xa") <<     pp  << oc << cc;          // eXtra Actor
                const rule &ac    = agg("ac") << fa << zom(xa);                // ACtors

                //--------------------------------------------------------------
                // describe components lhs/rhs
                //--------------------------------------------------------------
                const rule &ca    = choice(term('.'),ac);
                const rule &cm    = agg("cm") << opt( cat(ca,mark("<=>")) ) << ca;

                //--------------------------------------------------------------
                // describe equilibrium
                //--------------------------------------------------------------
                const rule &sep = mark(':');
                const rule &str = load<jive::lexical::rstring>("str");
                const rule &eq  =
                agg("eq") << id << sep << cm << sep << str;

                //--------------------------------------------------------------
                // expression to load database
                //--------------------------------------------------------------
                const rule &rx = load<jive::lexical::bstring>("rx");

                //--------------------------------------------------------------
                // possible input
                //--------------------------------------------------------------
                const rule &decl = cat( choice(eq,sp,rx), opt( mark(';') ) );
                top( agg("weasel") << zom( decl ) );

                //--------------------------------------------------------------
                // lexical only extra
                //--------------------------------------------------------------
                plug( jive::lexical::cxx_comment::clid, "C++Comment");
                plug( jive::lexical::c_comments::clid,  "C_Comments");
                drop("[:blank:]+");
                endl("[:endl:]");

                //--------------------------------------------------------------
                //
                //--------------------------------------------------------------
                validate();
                

                if(false)
                {
                    list<string> terminals, internals;
                    collect_keywords(terminals,internals);
                    std::cerr << "terminals: " << terminals << std::endl;
                    std::cerr << "internals: " << internals << std::endl;

                }
            }

        }

    }

}


