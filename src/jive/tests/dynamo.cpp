
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/translator.hpp"

#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/lexical/plugin/rstring.hpp"

namespace yack
{
    namespace jive
    {

        class dynamo : public parser
        {
        public:
            virtual ~dynamo() noexcept {}

            explicit dynamo() : parser( "dynamo" )
            {
                const rule &SEP     = mark(':');
                const rule &END     = mark(';');
                const rule &UID     = term("UID","\\.[:word:]+");
                const rule &UID_    = act("UID_") << UID << END;

                const rule &RID     = term("RID","[:word:]+");

                const rule &DCL     = act("DCL") << RID << SEP;
                const rule &JSTR    = load<lexical::jstring>("JSTR");
                const rule &RSTR    = load<lexical::rstring>("RSTR");

                const rule &ALIAS   = agg("ALIAS") << DCL << choice(RID,JSTR,RSTR) << END;

                top( agg("dynamo") << UID_ << zom(ALIAS) );
                gv();



                plug( jive::lexical::cxx_comment::clid, "C++Comment");
                plug( jive::lexical::c_comments::clid,  "C_Comments");
                drop("[:blank:]+");
                endl("[:endl:]");
            }
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dynamo);
        };

    }
}

using namespace yack;
using namespace jive;


YACK_UTEST(dynamo)
{
    jive::syntax::rule::verbose = true;
    dynamo dyn;

    if(argc>1)
    {
        const string                  fn = argv[1];
        auto_ptr<jive::syntax::xnode> tree = dyn.parse(module::open_file(fn));
        YACK_CHECK(tree.is_valid());
        tree->gv("tree.dot");
        jive::syntax::translator xt;
        xt.walk(*tree,NULL);
    }

}
YACK_UDONE()



