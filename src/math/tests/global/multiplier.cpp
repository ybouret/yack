#include "yack/math/multiplier.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace yack
{
    namespace math
    {
      
        
    }
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
        int         exx = 0;
        long double sxx = std::frexp(x,&exx);
        std::cerr << x << " -> " << sx << "*2^" << ex << std::endl;
        std::cerr << x << " -> " << sxx << "*2^" << exx << std::endl;

    }

    {
        math::multiplier<float> mm;
        mm << 1e-16 << 1e27 << 4.0f;
        std::cerr << "mm=" << mm  << std::endl;
    }

}
YACK_UDONE()

