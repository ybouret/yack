
#include "yack/memory/allocator/dyadic.hpp"

#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(memory_dyadic)
{
    memory::dyadic &P = memory::dyadic::instance();
    P.display();
}
YACK_UDONE()

