#include "yack/type/fourcc.h"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(type_fourcc)
{
    const uint32_t code = YACK_FOURCC('a', 'b', 'c', 'd');
    std::cerr << "fourcc=" << yack_fourcc(code) << std::endl;

}
YACK_UDONE()

