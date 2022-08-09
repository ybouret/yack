#include "yack/ios/bits.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_bits)
{
    io_bits Q;

    Q.append<int>(3,4);
    YACK_CHECK(Q.size==4);

    std::cerr << Q << std::endl;

}
YACK_UDONE()
