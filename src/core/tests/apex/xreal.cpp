#include "yack/apex/xreal.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace apex;

YACK_UTEST(apex_xreal)
{
    xreal<float> f(4);
    std::cerr << f << std::endl;

    for(int i=-10;i<=10;++i)
    {
        std::cerr << "10^" << i << " => " << xreal<double>::ten_to(i) << std::endl;
    }
}
YACK_UDONE()

