

#include "yack/chem/designer/cm-parser.hpp"
#include "yack/chem/designer/com.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {

        namespace nucleus
        {

            cm_parser:: ~cm_parser() noexcept {}
            
            cm_parser:: cm_parser() : jive:: parser("chemical::components::parser")
            {

                compound   &COMPONENTS = agg(COM::COMPONENTS);
                compound   &COMPONENT  = grp("COMPONENT");
                const rule &POS        = term('+');
                const rule &NEG        = term('-');
                const rule &SGN        = choice(POS,NEG);
                const rule &XPOS       = oom(POS);
                const rule &XNEG       = oom(NEG);
                COMPONENT << mark('[') << ( agg(COM::SPECIES) << term(COM::SP_NAME,COM::SP_EXPR) << opt( choice(XPOS,XNEG) ) ) << mark(']');

                const rule &COEF       = term(COM::COEF,"[1-9][0-9]*");
                const rule &FIRST_COEF = (agg(COM::FIRST_COEF) << choice(cat(SGN,COEF),SGN,COEF));
                const rule &EXTRA_COEF = (agg(COM::EXTRA_COEF) << cat(SGN,opt(COEF)));
                
                COMPONENTS << opt(FIRST_COEF) << COMPONENT;
                COMPONENTS << zom( cat(EXTRA_COEF,COMPONENT) );

                drop("[:blank:]+");

                validate();
                
                if(false)
                {
                    vector<string> terminals;
                    vector<string> internals;
                    collect_keywords(terminals,internals);
                    std::cerr << "terminals=" << terminals << std::endl;
                    std::cerr << "internals=" << internals << std::endl;
                }
            }

        }
    }

}
