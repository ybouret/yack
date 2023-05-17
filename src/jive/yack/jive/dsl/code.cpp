#include "yack/jive/dsl/code.hpp"

namespace yack
{
    namespace jive
    {

        //----------------------------------------------------------------
        // keywords
        //----------------------------------------------------------------
        const char *YACK_DSL_Terminals[] = {
            "+",
            "*",
            "?",
            "JSTRING",
            "RSTRING",
            "BSTRING",
            "MODULE_ID",
            "RID",
            "PID",
            "LID"
        };

        //----------------------------------------------------------------
        // keywords
        //----------------------------------------------------------------
        const char *YACK_DSL_Internals[] = {
            "MODULE",
            "ALT",
            "AGG",
            "JRK",
            "RULE",
            "PLG",
            "LEX",
            "INC"
        };



        dsl_code:: dsl_code() :
        terminal_hash(YACK_DSL_Terminals,sizeof(YACK_DSL_Terminals)/sizeof(YACK_DSL_Terminals[0])),
        internal_hash(YACK_DSL_Internals,sizeof(YACK_DSL_Internals)/sizeof(YACK_DSL_Internals[0])),
        nref(0)
        {

        }

        dsl_code:: ~dsl_code() noexcept
        {

        }

        YACK_COUNTED_IMPL(dsl_code,nref)

    }

}


