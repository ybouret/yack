#include "yack/apex.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apex_isqrt)
{

    for(apn n = 0;n<=100;++n)
    {
        std::cerr << n << " => " << apn::sqrt(n) << std::endl;
    }

}
YACK_UDONE()
