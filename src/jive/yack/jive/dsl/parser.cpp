
#include "yack/jive/dsl/parser.hpp"
#include "yack/jive/lexical/plugin.hpp"

#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/lexical/plugin/rstring.hpp"
#include "yack/jive/lexical/plugin/bstring.hpp"


namespace yack
{
    namespace jive
    {

#define DSL_IDENTIFIER_RX "[_[:alpha:]][:word:]*"


        const char dsl_parser:: _include[] = "#include";

        static const char * strids[] = {
            "JSTRING",
            "RSTRING",
            "BSTRING"
        };

        static const char * topids[] = {
            "INC",
            "RULE"
        };


        dsl_parser:: dsl_parser() :
        parser("dsl_parser"),
        str2type(strids,sizeof(strids)/sizeof(strids[0])),
        top2type(topids,sizeof(topids)/sizeof(topids[0]))
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

            const rule &JSTRING  = load<jive::lexical::jstring>("JSTRING");
            const rule &RSTRING  = load<jive::lexical::rstring>("RSTRING");
            const rule &BSTRING  = load<jive::lexical::bstring>("BSTRING");
            const rule &ASTRING  = alt("ASTRING") << JSTRING << RSTRING;
            const rule &STRINGS  = oom(ASTRING);

            //------------------------------------------------------------------
            //
            // a module starts with a MODULE_ID
            //
            //------------------------------------------------------------------
            MODULE << cat(term("MODULE_ID","[.]" DSL_IDENTIFIER_RX ),END);

            compound &CONTENT = alt("CONTENT");
            

            //------------------------------------------------------------------
            //
            // defining a rule
            //
            //------------------------------------------------------------------

            {
                const rule &RID       = term("RID",DSL_IDENTIFIER_RX); // Rule IDentifier
                compound   &ALTERNATE = act("ALT");
                compound   &AGGREGATE = act("AGG");
                compound   &ITEM      = alt("ITM");
                compound   &JOKER     = act("JRK");

                ALTERNATE << AGGREGATE << zom(cat(ALTERN,AGGREGATE));
                AGGREGATE << JOKER << zom(JOKER);
                ITEM      << RID   << ASTRING << cat(LPAREN,ALTERNATE,RPAREN);
                JOKER     << ITEM  << opt(choice(OOM,ZOM,OPT));

                CONTENT << (agg("RULE") << RID << SEP << ALTERNATE << END);
            }


            //------------------------------------------------------------------
            //
            // defining a plugin
            //
            //------------------------------------------------------------------
            CONTENT << (agg("PLG") << term("PID","@" DSL_IDENTIFIER_RX) << SEP << STRINGS << END);

            //------------------------------------------------------------------
            //
            // defining a lexical only rule
            //
            //------------------------------------------------------------------
            CONTENT << (agg("LEX") << term("LID","%" DSL_IDENTIFIER_RX) << SEP << STRINGS << END);



            //------------------------------------------------------------------
            //
            // defining an include
            //
            //------------------------------------------------------------------
            CONTENT << (agg("INC") << mark(_include) <<  choice(JSTRING,BSTRING));


            //------------------------------------------------------------------
            //
            // feeding module with one or more content
            //
            //------------------------------------------------------------------
            MODULE << zom(CONTENT);


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

            validate();
        }

        dsl_parser:: ~dsl_parser() noexcept
        {
            
        }

       
      
    }

}

