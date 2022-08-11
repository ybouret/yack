

#include "yack/chem/designer/cm-parser.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {

        namespace nucleus
        {

            cm_parser:: ~cm_parser() throw() {}

            static const char ch_expr[] = "[-+[:word:]]";
            

            cm_parser:: cm_parser() : jive:: parser("chemical::components::parser")
            {
                compound   &COMPONENTS = agg("COMPONENTS");
                compound   &COMPONENT  = ( agg("COMPONENT") << mark('[') << oom( term("CH",ch_expr) ) << mark(']') );
                const rule &COEFF      = term("COEFF","[1-9][0-9]*");
                
                COMPONENTS << COMPONENT;

                COMPONENTS << zom( COMPONENT );

                drop("[:blank:]+");

                gv();
            }

        }
    }

}
