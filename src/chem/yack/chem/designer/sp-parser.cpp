
#include "yack/chem/designer/sp-parser.hpp"
#include "yack/chem/designer/com.hpp"

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
                syntax::compound &SPECIES = agg(COM::SPECIES);
                SPECIES << term(COM::SP_NAME,COM::SP_EXPR);
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
