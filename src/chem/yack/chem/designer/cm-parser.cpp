

#include "yack/chem/designer/cm-parser.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {

        namespace nucleus
        {

            cm_parser:: ~cm_parser() throw() {}

            static const char ch_expr[] = "[^\\[\\]]";
            

            cm_parser:: cm_parser() : jive:: parser("chemical::components::parser")
            {
                compound   &COMPONENTS = agg("COMPONENTS");
                compound   &COMPONENT  = ( agg("COMPONENT") << mark('[') << oom( term("CH",ch_expr) ) << mark(']') );

                COMPONENTS << COMPONENT;

                drop("[:blank:]");
                gv();
            }

        }
    }

}
