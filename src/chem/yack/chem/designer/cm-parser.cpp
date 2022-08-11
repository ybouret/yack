

#include "yack/chem/designer/cm-parser.hpp"
#include "yack/chem/designer/com.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {

        namespace nucleus
        {

            cm_parser:: ~cm_parser() throw() {}



            cm_parser:: cm_parser() : jive:: parser("chemical::components::parser")
            {
#if 0
                compound   &COMPONENTS = agg("COMPONENTS");
                compound   &COMPONENT  = ( agg("COMPONENT") << mark('[') << oom( term("CH",ch_expr) ) << mark(']') );
                const rule &COEFF      = term("COEFF","[1-9][0-9]*");
                
                COMPONENTS << COMPONENT;

                COMPONENTS << zom( COMPONENT );
#endif
                
                compound   &COMPONENTS = agg("COMPONENTS");
                compound   &COMPONENT = agg("COMPONENT");
                const rule &POS       = term('+');
                const rule &NEG       = term('-');
                const rule &XPOS      = oom(POS);
                const rule &XNEG      = oom(NEG);
                COMPONENT << mark('[') << ( agg(COM::SPECIES) << term(COM::SP_NAME,COM::SP_EXPR) << opt( choice(XPOS,XNEG) ) ) << mark(']');

                COMPONENTS << COMPONENT;

                COMPONENTS << zom(COMPONENT);

                drop("[:blank:]+");

                gv();
            }

        }
    }

}
