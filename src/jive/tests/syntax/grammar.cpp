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
            const rule &ID     = term_("ID");
            const rule &INT    = term_("INT");
            const rule &OPT_ID = opt(ID);
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
    std::cerr << "#rules=" << (*G).size << std::endl;

    G.gv();


}
YACK_UDONE()

