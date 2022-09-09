
#include "yack/math/algebra/crout_.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/complex.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(crout)
{
    size_t nmax = 5;
    if(argc>1)
    {
        nmax = ios::ascii::convert::to<size_t>(argv[1]);
    }

    for(size_t n=0;n<=nmax;++n)
    {
        crout_ cr(n,sizeof(float),sizeof(complex<float>));

        std::cerr << "dims=" << cr.dims << ", granted=" << cr.granted() << std::endl;
    }

}
YACK_UDONE()



