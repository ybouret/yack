
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

    straight_spline<float, v2d<float> > S(4);
    periodic_spline<float, v2d<float> > P(4);

    const size_t np = 1+ran.leq(10);
    const float  da = 6.28f/(np+1);
    for(size_t i=np;i>0;--i)
    {
        const float     alpha = i*da + 0.3f * da* ran.symm<float>();
        const v2d<float> v = v2d<float>( cos(alpha) + 0.1f * ran.symm<float>(),
                                        sin(alpha) + 0.1f * ran.symm<float>());
        S.add(alpha,v);
        P.add(alpha,v);
    }

    S.sort();
    P.sort();

    std::cerr << "s.x=" << S.x << std::endl;
    std::cerr << "s.y=" << S.y << std::endl;

    {
        ios::ocstream fp("spline.dat");
        for(size_t i=1;i<=S.size();++i)
        {
            fp("%g",S.x[i]);
            S.print(fp,S.y[i]);
            fp << "\n";
        }
    }

    S.build();
    
    {
        ios::ocstream fp("splfcn.dat");
        const size_t N = 100;
        for(size_t i=0;i<=N;++i)
        {
            const double     xx = S.lower() + ((S.upper()-S.lower()) * i)/N;
            const v2d<float> yy = S(xx);
            fp("%g",xx);
            S.print(fp,yy);
            fp << "\n";
        }
    }

    
    P.build(P.x[1]-da,P.x[np]+da);

    {
        ios::ocstream fp("splper.dat");
        std::cerr << "P.upper=" << P.upper() << " / P.end=" << P.x.back() << std::endl;
        std::cerr << "P.lower=" << P.lower() << " / P.ini=" << P.x.front() << std::endl;

        const size_t N = 100;
        for(size_t i=0;i<=N;++i)
        {
            const double     xx = P.lower() + ((P.upper()-P.lower()) * i)/N;
            const v2d<float> yy = P(xx);
            fp("%g",xx);
            P.print(fp,yy);
            fp << "\n";
        }
    }

}
YACK_UDONE()

