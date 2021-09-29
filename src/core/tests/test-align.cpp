
#include "yack/arith/align.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

YACK_UTEST(align)
{
    for(size_t i=0;i<=100;++i)
    {
        std::cerr << i << std::endl;
        for(size_t j=0;j<=4;++j)
        {
            std::cerr << ' ' << YACK_ALIGN_LN2(j,i) << std::endl;
        }

    }
}
YACK_UDONE()

