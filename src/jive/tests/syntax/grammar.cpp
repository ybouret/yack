#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/utest/run.hpp"
#include "yack/associative/addrbook.hpp"

using namespace yack;

namespace
{
    class Gram : public jive::syntax::grammar
    {
    public:
        explicit Gram() : jive::syntax::grammar( "JSON" )
        {

            compound   &value  = alt("value");
            const rule &STRING = term_("string");
            value << STRING;

            value << term_("number") << term_("false") << term_("true");





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
    std::cerr << G.lang << " #rules=" << (*G).size << std::endl;

    G.gv();

    YACK_SIZEOF(jive::syntax::grammar);
    YACK_SIZEOF(addrbook);
}
YACK_UDONE()

