
#include "yack/utest/run.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/part.hpp"
#include "yack/counting/perm.hpp"

using namespace yack;


#define SHOW_EXPR(EXPR) std::cerr << "#" << #EXPR << " = " << (EXPR) << std::endl

YACK_UTEST(counting_card)
{

    SHOW_EXPR( counting::card<partition>::of(5) );
    SHOW_EXPR( counting::card<partition>::of(10) );
    SHOW_EXPR( counting::card<permutation>::of(10) );
    SHOW_EXPR( counting::card<combination>::of(12,6) );

}
YACK_UDONE()


