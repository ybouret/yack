
#include "yack/memory/allocator/pages.hpp"

#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(memory_pages)
{

    memory::pages &P = memory::pages::instance();
    P.display();
}
YACK_UDONE()

