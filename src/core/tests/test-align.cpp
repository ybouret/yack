
#include "yack/arith/align.hpp"
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
            const size_t II = alignment::with_ln2(j,I);
            YACK_ASSERT(I==II);
            YACK_CHECK(I==II);
            std::cerr << ' ' << I << "/" << II << std::endl;
        }
    }



}
YACK_UDONE()

