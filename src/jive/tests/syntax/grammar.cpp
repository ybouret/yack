#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class Gram : public jive::syntax::grammar
    {
    public:
        explicit Gram() : jive::syntax::grammar( "Gram" )
        {
            const rule &ID =  term_("ID");
            (void)ID;
            const rule &OPT_ID = opt(ID);
            (void)OPT_ID;

            const rule &ZOM_ID = zom(ID);
            (void)ZOM_ID;


        }

        virtual ~Gram() throw()
        {
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Gram);
    };
}

YACK_UTEST(grammar)
{
    Gram G;

    G.gv();


}
YACK_UDONE()

