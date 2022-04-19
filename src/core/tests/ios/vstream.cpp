#include "yack/ios/ovstream.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"

using namespace yack;

YACK_UTEST(ios_vstream)
{
    ios::ovstream<memory::dyadic> s;

    s << "Hello, World!";

    std::cerr << s.buffer << std::endl;
}
YACK_UDONE()

