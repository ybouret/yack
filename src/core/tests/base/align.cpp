
#include "yack/arith/align.h"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(align)
{
    for(size_t i=0;i<=100;++i)
    {
        std::cerr << i << std::endl;
        for(size_t j=0;j<=4;++j)
        {
            const size_t I  = YACK_ALIGN_LN2(j,i);
            std::cerr << ' ' << I <<  std::endl;
        }
    }



}
YACK_UDONE()

