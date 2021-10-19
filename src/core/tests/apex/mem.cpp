
#include "yack/apex/m/vein.hpp"
#include "yack/memory/small/arena.hpp"
#include "yack/utest/run.hpp"
#include <cstdio>

using namespace yack;

namespace
{
    
    
}

YACK_UTEST(apex_mem)
{
    uprng ran;
    YACK_SIZEOF(apex::vein);
    YACK_OUTPUT(apex::vein::min_blocks);
    YACK_OUTPUT(apex::vein::max_blocks);
    YACK_SIZEOF(memory::arena);
}
YACK_UDONE()
