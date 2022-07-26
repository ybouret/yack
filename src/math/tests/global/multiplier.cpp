#include "yack/utest/run.hpp"

using namespace yack;

namespace
{

}

YACK_UTEST(multiplier)
{

    {
        double x = 1.012;
        int    ex = 0;
        double sx = frexp(x,&ex);
        std::cerr << x << " -> " << sx << "*2^" << ex << std::endl;
    }

    {
        float x = -9e-16;
        int   ex = 0;
        float sx = frexpf(x,&ex);
        std::cerr << x << " -> " << sx << "*2^(" << ex << ")" << std::endl;
    }


    {
        long double x  = 3.14L;
        int         ex = 0;
        long double sx = frexpl(x,&ex);
        std::cerr << x << " -> " << sx << "*2^" << ex << std::endl;
    }


}
YACK_UDONE()

