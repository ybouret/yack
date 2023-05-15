
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

            const rule &OOM       = term('+');
            const rule &ZOM       = term('*');
            const rule &OPT       = term('?');

            const rule &JS  = load<jive::lexical::jstring>("JS");
            const rule &RS  = load<jive::lexical::jstring>("RS");
            const rule &STR = alt("STR") << JS << RS;

            //------------------------------------------------------------------
            //
            // a module starts with a MODULE_ID
            //
            //------------------------------------------------------------------
            MODULE << cat(term("MODULE_ID","[.]" DSL_IDENTIFIER_RX ),END);

            

            //------------------------------------------------------------------
            //
            // defining a rule
            //
            //------------------------------------------------------------------
            compound &RULE  = agg("RULE");
            {
                const rule &RID     = term("RID",DSL_IDENTIFIER_RX); // Rule IDentifier
                compound   &ALT     = act("ALT");
                compound   &SXP     = act("SXP");
                compound   &ATOM    = alt("ATOM");
                compound   &JKR     = act("JKR");

                ALT  << SXP  << zom(cat(ALTERN,SXP));
                SXP  << JKR << zom(JKR);
                ATOM << RID << JS << RS << cat(LPAREN,ALT,RPAREN);
                JKR  << ATOM << opt(choice(OOM,ZOM,OPT));
                RULE << RID << SEP << ALT << END;
            }



            //------------------------------------------------------------------
            //
            // defining a plugin
            //
            //------------------------------------------------------------------
            compound &PLUGIN = agg("PLUGIN") << term("PID","@" DSL_IDENTIFIER_RX) << SEP << oom(STR) << END;


            MODULE << zom(choice(RULE,PLUGIN));


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

