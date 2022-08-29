#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

YACK_UTEST(apfac)
{

    for(int i=1;i<argc;++i)
    {
        const unsigned n = ios::ascii::convert::to<unsigned>(argv[1],"n");
        std::cerr << n << "! = ";
        const apn f = apn::factorial(n);
        std::cerr << f;
        std::cerr << std::endl;
    }


}
YACK_UDONE()
