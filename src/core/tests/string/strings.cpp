#include "yack/string/string.hpp"
#include "yack/string/utf8.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(strings)
{
    kernel::string<char> s1; std::cerr << "s1.width=" << s1.width << std::endl;
    kernel::string<utf8> s2; std::cerr << "s2.width=" << s2.width << std::endl;

}
YACK_UDONE()

