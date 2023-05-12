
#include "yack/jive/dsl/parser.hpp"
#include "yack/jive/lexical/plugin.hpp"

#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/lexical/plugin/rstring.hpp"


namespace yack
{
    namespace jive
    {

#define DSL_IDENTIFIER_RX "[_[:alpha:]][:word:]*"

        dsl_parser:: dsl_parser() : parser("dsl_parser")
        {
            // top level MODULE
            compound   &MODULE    = agg("MODULE");

            const rule &END       = mark(';');
            const rule &SEP       = mark(':');
            // a module stat with a MODULE_ID
            MODULE << cat(term("MODULE_ID","[.]" DSL_IDENTIFIER_RX ),END);


            const rule &RULE_ID = term("RULE_ID",DSL_IDENTIFIER_RX);       // Rule IDentifier
            const rule &JSTRING = load<jive::lexical::jstring>("JSTRING"); // JSTRING
            const rule &RSTRING = load<jive::lexical::jstring>("RSTRING"); // RSTRING
            const rule &STRING  = choice(JSTRING,RSTRING);
            

            

            // comments
            plug( jive::lexical::cxx_comment::clid, "C++Comment");
            plug( jive::lexical::c_comments::clid,  "C_Comments");

            // lexical only
            drop("[:blank:]+");
            endl("[:endl:]");

            gv();
        }


        dsl_parser:: ~dsl_parser() noexcept
        {
            
        }

    }

}

