
#include "yack/gfx/blur.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace graphic;

YACK_UTEST(blur)
{
    float sigma = 1.0f;
    if( argc > 1)
    {
        sigma = ios::ascii::convert::real<float>(argv[1],"sigma");
    }
    blurring B(sigma);
    std::cerr << "sigma=" << sigma << " => radius=" << B.radius << std::endl;
    std::cerr << B << std::endl;
}
YACK_UDONE()

