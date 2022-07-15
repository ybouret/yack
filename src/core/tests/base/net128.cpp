#include "yack/type/uint128.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(net128)
{
    { uint128_t x; std::cerr << x << std::endl; }

    { uint128_t x = 0x1234; std::cerr << x << std::endl; }


}
YACK_UDONE()

