#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apr)
{
    if(argc>2)
    {
        const apn num = apn::parse(argv[1]);
        const apn den = apn::parse(argv[2]);
        std::cerr << num << "/" << den << " =";
        const float f = apn::ratio<float>(num,den);       std::cerr << " " << f;
        const float d = apn::ratio<double>(num,den);      std::cerr << " " << d;
        const float l = apn::ratio<long double>(num,den); std::cerr << " " << l;

        std::cerr << std::endl;

    }
}
YACK_UDONE()

