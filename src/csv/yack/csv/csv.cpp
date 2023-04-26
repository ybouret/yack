
#include "yack/csv/csv.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"

namespace yack
{

    namespace CSV
    {
        Parser:: ~Parser() noexcept
        {
        }



        Parser:: Parser() : jive::parser("CSV")
        {

            const rule &RAW = term("RAW","[ \t[:word:]']+");
            const rule &STR = load<jive::lexical::jstring>("STR");

            const rule &FIELD = choice(RAW,STR);
            const rule &SEP   = mark(',');
            const rule &ENDL  = EOL("ENDL", "[:endl:]",false);
            const rule &LINE  = agg("LINE") << FIELD << zom(cat(SEP,FIELD)) << ENDL;

            top( zom(LINE) );
            validate();
            gv();
        }
    }

}
