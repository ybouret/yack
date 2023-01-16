#include "yack/system/arch.h"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(native)
{
    std::cerr << YACK_ARCHITECTURE << std::endl;
    std::cerr << YACK_NATIVE_BITS  << std::endl;
}
YACK_UDONE()
