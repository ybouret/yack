#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/ios/ocstream.hpp"

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

    ios::ocstream fp("aplnfac.dat");
    for(unsigned n=0;n<=100;++n)
    {
        const apn    f  = apn::factorial(n);
        const double lf = apn::log_of<double>(f);
        fp("%u %.15g\n",n,lf);
    }

    for(size_t n=0; ;++n)
    {
        const apn    f  = apn::factorial(n);
        const size_t b = f.bits();
        std::cerr << n << "! = " << f << " [" << b << "]" << std::endl;
        if(b>64) break;
    }

}
YACK_UDONE()
