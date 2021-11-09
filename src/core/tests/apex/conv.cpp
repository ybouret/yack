

#include "yack/apex.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(apex_conv)
{
    apn n = 1000;
    std::cerr << n.cast_to<int>() << std::endl;
    apz z = -100;
    std::cerr << int(z.cast_to<int8_t>()) << std::endl;

}
YACK_UDONE()

