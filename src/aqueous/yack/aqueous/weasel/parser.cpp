
#include "yack/aqueous/weasel/parser.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            parser:: ~parser() noexcept
            {
            }

            parser:: parser() : jive::parser("weasel")
            {
                const rule &lbrack = mark('[');
                const rule &rbrack = mark(']');
                const rule &spname = term("sp","[:alpha:][:word:]*");

                
                // lexical only extra
                drop("[:blank:]+");
                endl("[:endl:]");
            }

        }

    }

}


