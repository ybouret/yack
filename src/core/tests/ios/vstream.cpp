#include "yack/ios/ovstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_vstream)
{
    ios::ovstream<memory::dyadic> s;

    s << "Hello, World!";

    std::cerr << s.data << std::endl;
}
YACK_UDONE()

