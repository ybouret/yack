

#include "yack/apex.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(apex_conv)
{
    apn n = 100001;
    std::cerr << n.cast_to<int>() << std::endl;
    std::cerr << n.to<double>()    << std::endl;
    apz z = -100;
    std::cerr << int(z.cast_to<int8_t>()) << std::endl;

    {
        apn    num = 31;
        apn    den = 7;
        double ans = apn::ratio<double>(num,den);
        std::cerr << num << "/" << den << " = " << ans << std::endl;
    }

}
YACK_UDONE()

