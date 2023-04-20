
#include "yack/math/data/spline.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(spline)
{

    YACK_SIZEOF( tridiag<float> );
    YACK_SIZEOF( tridiag<double> );
    YACK_SIZEOF( tridiag<long double> );

    spline<float,float> s(4);

    s(1,1);
    s(1.6,5.5);
    s(2,4);

    s.sort();

    std::cerr << "s.x=" << s.x << std::endl;
    std::cerr << "s.y=" << s.y << std::endl;

    {
        ios::ocstream fp("spline.dat");
        for(size_t i=1;i<=s.size();++i)
        {
            fp("%g %g\n",s.x[i],s.y[i]);
        }
    }

    spline<float,float>::linear ls(s.size());

    ls(s);
    std::cerr << "s.y2=" << s.y2 << std::endl;


    {
        ios::ocstream fp("splfcn.dat");
        const size_t N = 100;
        for(size_t i=0;i<=N;++i)
        {
            const double xx = s.x[1] + ((s.x.back()-s.x.front()) * i)/N;
            const double yy = s(xx);
            fp("%g %g\n",xx,yy);
        }
    }



}
YACK_UDONE()

