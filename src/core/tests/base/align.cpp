
#include "yack/arith/align.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(align)
{
    for(size_t i=0;i<=100;++i)
    {
        std::cerr << i << " : ";
        std::cerr << ' ' << YACK_ALIGN_TO(uint8_t,i);
        std::cerr << ' ' << YACK_ALIGN_TO(uint16_t,i);
        std::cerr << ' ' << YACK_ALIGN_TO(uint32_t,i);
        std::cerr << ' ' << YACK_ALIGN_TO(uint64_t,i);
        std::cerr << ' ' << YACK_ALIGN_ON(16,i);
        std::cerr << std::endl;
    }
    
}
YACK_UDONE()

