
#include "yack/math/data/spline.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/type/v2d.hpp"
#include "../core/tests/main.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(spline)
{
    randomized::rand_ ran;

    YACK_SIZEOF( tridiag<float> );
    YACK_SIZEOF( tridiag<double> );
    YACK_SIZEOF( tridiag<long double> );

    YACK_SIZEOF( cyclic<float> );
    YACK_SIZEOF( cyclic<double> );
    YACK_SIZEOF( cyclic<long double> );

    spline<float, v2d<float> > s(4);

    float x_ = 0;
    for(size_t i=1+ran.leq(10);i>0;--i)
    {
        const v2d<float> v = bring::get< v2d<float> >( ran );
        x_ += 5 * ran.to<float>();
        s(x_,v);
    }
    s.sort();

    std::cerr << "s.x=" << s.x << std::endl;
    std::cerr << "s.y=" << s.y << std::endl;

    {
        ios::ocstream fp("spline.dat");
        for(size_t i=1;i<=s.size();++i)
        {
            fp("%g",s.x[i]);
            s.print(fp,s.y[i]);
            fp << "\n";
        }
    }

    spline<float,v2d<float> >::linear ls(s.size());

    ls(s);
    std::cerr << "s.y2=" << s.y2 << std::endl;


    {
        ios::ocstream fp("splfcn.dat");
        const size_t N = 100;
        for(size_t i=0;i<=N;++i)
        {
            const double     xx = s.x[1] + ((s.x.back()-s.x.front()) * i)/N;
            const v2d<float> yy = s(xx);
            fp("%g",xx);
            s.print(fp,yy);
            fp << "\n";
        }
    }



}
YACK_UDONE()

