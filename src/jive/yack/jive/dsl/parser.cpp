
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
            //------------------------------------------------------------------
            //
            // top level MODULE
            //
            //------------------------------------------------------------------
            compound   &MODULE    = agg("MODULE");

            //------------------------------------------------------------------
            //
            // common rules
            //
            //------------------------------------------------------------------
            const rule &END       = mark(';');
            const rule &SEP       = mark(':');
            const rule &LPAREN    = mark('(');
            const rule &RPAREN    = mark(')');
            const rule &ALTERN    = mark('|');

            //------------------------------------------------------------------
            //
            // a module starts with a MODULE_ID
            //
            //------------------------------------------------------------------
            MODULE << cat(term("MODULE_ID","[.]" DSL_IDENTIFIER_RX ),END);

            
            const rule &RULE_ID = term("RULE_ID",DSL_IDENTIFIER_RX);           // Rule IDentifier
            const rule &JSTRING = load<jive::lexical::jstring>("JSTRING"); // JSTRING
            const rule &RSTRING = load<jive::lexical::jstring>("RSTRING"); // RSTRING
            const rule &ASTRING = choice(JSTRING,RSTRING);
            
            compound   &ALT     = act("ALT");
            compound   &SXP     = act("SXP");
            compound   &ATOM    = alt("ATOM");

            ALT  << SXP  << zom(cat(ALTERN,SXP));
            SXP  << ATOM << zom(ATOM);
            ATOM << RULE_ID << JSTRING << RSTRING << cat(LPAREN,ALT,RPAREN);

            const rule &RULE  = agg("RULE") << RULE_ID << SEP << ALT << END;


            MODULE << zom(RULE);

            //------------------------------------------------------------------
            //
            // lexical comments
            //
            //------------------------------------------------------------------
            (void) plug( jive::lexical::cxx_comment::clid, "C++Comment");
            (void) plug( jive::lexical::c_comments::clid,  "C_Comments");
            
            //------------------------------------------------------------------
            //
            // lexical only
            //
            //------------------------------------------------------------------
            drop("[:blank:]+");
            endl("[:endl:]");

            gv();
        }


        dsl_parser:: ~dsl_parser() noexcept
        {
            
        }

    }

}

