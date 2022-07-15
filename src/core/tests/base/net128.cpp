#include "yack/type/net128.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(net128)
{
    { net128_t x; std::cerr << x << std::endl; }
    { net128_t x = 1; std::cerr << x << std::endl; }

}
YACK_UDONE()

