#include "yack/utest/run.hpp"
#include "yack/arith/gcd.h"

using namespace yack;

YACK_UTEST(gcd64)
{

    for(uint64_t a=0;a<=1000;++a)
    {
        for(uint64_t b=0;b<=1000;++b)
        {
            const uint64_t g = yack_gcd64(a,b);
            const uint64_t h = yack_gcd64(b,a);
            YACK_ASSERT(h==g);
        }
    }

    if(argc>2)
    {
        uint64_t a = atol(argv[1]);
        uint64_t b = atol(argv[2]);
        std::cerr << "gcd(" << a << "," << b << ")=" << yack_gcd64(a,b) << std::endl;
    }

}
YACK_UDONE()

