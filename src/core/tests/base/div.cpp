#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(div)
{
    ptrdiff_t prod = 10;
    ptrdiff_t d    = 3;
    ldiv_t    ans   = ldiv(prod,d);
    std::cerr << ans.quot << ", " << ans.rem << std::endl;

}
YACK_UDONE()

