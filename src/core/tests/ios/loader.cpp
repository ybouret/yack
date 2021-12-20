#include "yack/ios/ascii/converter.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_loader)
{
    ios::ascii::converter &ld = ios::ascii::converter::instance();
    std::cerr << ld.call_sign << std::endl;
}
YACK_UDONE()
