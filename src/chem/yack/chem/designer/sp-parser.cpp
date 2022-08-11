
#include "yack/chem/designer/sp-parser.hpp"

namespace yack
{
    using namespace jive;
    
    namespace chemical
    {

        namespace nucleus
        {

            sp_parser:: ~sp_parser() throw() {}

            sp_parser:: sp_parser() : jive:: parser("chemical::species::parser")
            {
                syntax::compound &SPECIES = agg("SPECIES");
                SPECIES << term("NAME", "[:upper:][:word:]*");
                {
                    const syntax::rule &POS = oom( term('+') );
                    const syntax::rule &NEG = oom( term('-') );
                    SPECIES << opt( choice(POS,NEG) );
                }
                validate();
            }

        }
    }

}
