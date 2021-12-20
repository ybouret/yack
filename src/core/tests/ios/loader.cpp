#include "yack/ios/ascii/loader.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_loader)
{
    ios::ascii::loader &ld = ios::ascii::loader::instance();
    std::cerr << ld.call_sign << std::endl;
}
YACK_UDONE()
