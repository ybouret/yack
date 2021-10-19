
#include "yack/apex/m/slab.hpp"
#include "yack/utest/run.hpp"
#include <cstdio>

using namespace yack;


YACK_UTEST(apex_mem)
{
    uprng ran;
    
    YACK_SIZEOF(apex::slab::item_t);
    YACK_SIZEOF(apex::slab);
    std::cerr << "min size: " << 4 * sizeof(apex::slab::item_t) + 4 * sizeof(void*) << std::endl;
    
    
}
YACK_UDONE()
