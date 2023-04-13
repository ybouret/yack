#include "yack/apex/xreal.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace apex;

YACK_UTEST(apex_xreal)
{
    xreal<float> prod = 1;
    for(int i=1;i<argc;++i)
    {
        const float        r = ios::ascii::convert::real<float>(argv[i]);
        const xreal<float> f = r;
        std::cerr << r << " => " << f << std::endl;
        prod *= f;
    }
    std::cerr << "prod=" << prod << std::endl;

    xreal<float> a = 100;
    xreal<float> b = 0.01;
    std::cerr << "c=" << a*b << std::endl;

}
YACK_UDONE()

