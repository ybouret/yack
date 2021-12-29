
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

using namespace yack;

namespace
{
    class Eval : public jive::parser
    {
    public:
        explicit Eval() : jive::parser("Eval")
        {

            plug( jive::lexical::cxx_comment::clid, "C++Comment");
            plug( jive::lexical::c_comments::clid,  "C_Comments");
            drop("[:blank:]+");
            endl("[:endl:]");
        }

        virtual ~Eval() throw()
        {

        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Eval);
    };

}

YACK_UTEST(eval)
{
    Eval eval;

}
YACK_UDONE()

