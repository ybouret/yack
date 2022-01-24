
#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

using namespace yack;

namespace
{
    class Eval : public jive::parser
    {
    public:
        explicit Eval() : jive::parser("eval")
        {

            compound   &ATOM    = alt("atom");
            compound    &POW    = act("pow") << ATOM; POW << opt( cat( mark('^'), POW) );
            const rule  &MUL    = term('*');
            const rule  &DIV    = term('/');
            compound    &MULOP  = act("mulop") << POW << zom( cat( choice(MUL,DIV), POW ) );

            const rule  &ADD    = term('+');
            const rule  &SUB    = term('-');
            compound    &ADDOP  = act("addop") << MULOP << zom( cat( choice(ADD,SUB), MULOP) );

            ATOM << term("number","[:digit:]+");
            ATOM << cat( mark('('), ADDOP , mark(')') );

            top( zom(agg("expr") << ADDOP << mark(';')) );

            gv();
            validate();
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
    jive::syntax::rule::verbose = true;

    if( argc>1 )
    {
        jive::source                  src( jive::module::open_file(argv[1]) );
        auto_ptr<jive::syntax::xnode> tree = eval.parse(src);

        if(tree.is_valid())
        {
            tree->gv("tree.dot");
        }
    }


}
YACK_UDONE()

