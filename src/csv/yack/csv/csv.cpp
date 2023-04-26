
#include "yack/csv/csv.hpp"

namespace yack
{

    namespace CSV
    {
        Parser:: ~Parser() noexcept
        {
        }

        Parser:: Parser() : jive::parser("CSV")
        {

            const rule &FIELD = term("FIELD","[:word:]+");
            //const rule &SEP   = mark(',');
            const rule &ENDL  = endline("ENDL", "[:endl:]",false);
            const rule &LINE  = agg("LINE") << FIELD << ENDL;

            top( zom(LINE) );
            validate();
            gv();
        }
    }

}
