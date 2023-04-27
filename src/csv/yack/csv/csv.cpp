
#include "yack/csv/csv.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"

namespace yack
{

    namespace CSV
    {
        Parser:: ~Parser() noexcept
        {
        }



        Parser:: Parser() : jive::parser("CSV") ,
        tr( new jive::syntax::translator() )
        {

            const rule &RAW = term("RAW","[[:word:]']+");
            const rule &STR = load<jive::lexical::jstring>("STR");

            const rule &FIELD = choice(STR,RAW);
            const rule &SEP   = mark(',');
            const rule &ENDL  = EOL("ENDL", "[:endl:]",false);
            const rule &LINE  = agg("LINE") << FIELD << zom(cat(SEP,FIELD)) << ENDL;

            top( zom(LINE) );
            drop("[:blank:]");
            validate();
            gv();
        }
    }

}
