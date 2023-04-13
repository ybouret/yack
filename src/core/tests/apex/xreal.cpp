#include "yack/apex/xreal.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;
using namespace apex;

YACK_UTEST(apex_xreal)
{
    vector< xreal<float> > xr;
    for(int i=1;i<argc;++i)
    {
        const float        r = ios::ascii::convert::real<float>(argv[i]);
        const xreal<float> f = r;
        std::cerr << r << " => " << f << std::endl;
        xr << f;
    }
    std::cerr << xr << std::endl;

    {
        xreal<float> prod = 1;
        for(size_t i=xr.size();i>0;--i) prod *= xr[i];
        std::cerr << "prod=" << prod << std::endl;
    }

    for(size_t i=1;i<=xr.size();++i)
    {
        const xreal<float> &num = xr[i];
        for(size_t j=i;j<=xr.size();++j)
        {
            const xreal<float> &den = xr[j];
            if(fabs(den.m)<=0) continue;
            const xreal<float> q = num / den;
            const xreal<float> p = q * den;
            std::cerr << num << "/" << den << " = " << q << " => " << p << std::endl;
        }
    }

    if(xr.size()>0)
    {
        xreal<float> sum = xr[1];
        for(size_t i=2;i<=xr.size();++i)
        {
            sum += xr[i];
        }

    }


}
YACK_UDONE()

