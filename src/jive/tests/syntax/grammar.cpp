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

            const rule &LBRACK = term_('[');
            const rule &RBRACK = term_(']');
            const rule &COMA   = term_(',');


            compound   &array       = alt("array");
            {
                const rule &empty_array = ( agg("empty_array") << LBRACK << RBRACK);
                array << empty_array;
            }
            {
                const rule &heavy_array = ( agg("heavy_array") << LBRACK << value << oom( cat(COMA,value) ) << RBRACK );
                array << heavy_array;
            }

            top(array);
            gv();
            validate();
        }

        virtual ~Gram() noexcept
        {
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(Gram);
    };
}

YACK_UTEST(grammar)
{
    jive::lexical::scanner::verbose  = true;
    Gram G;
    std::cerr << G.lang << " #rules=" << (*G).size << std::endl;


}
YACK_UDONE()

