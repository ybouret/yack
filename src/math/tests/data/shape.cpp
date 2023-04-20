
#include "yack/math/data/spline.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/type/v2d.hpp"
#include "../core/tests/main.hpp"

using namespace yack;
using namespace math;

typedef v2d<double> V2D;
typedef periodic_spline<double,V2D> Spline;

static inline void save_spline(const char *fn, const Spline &P)
{
    ios::ocstream fp(fn);
    const size_t np = 100;
    for(size_t i=0;i<=np;++i)
    {
        const double x = (P.period * i)/np;
        const V2D    v = P(x);
        fp("%g %g %g\n",x,v.x,v.y);
    }
}



YACK_UTEST(shape)
{
    randomized::rand_ ran;

    Spline P(16);
    YACK_CHECK(2==P.dimension);

    const double a = 1.0;
    const double b = 0.8;

    const size_t np    = 8;
    const double alpha = 0.3;
    const double beta  = 6.28-alpha;
    const double delta = beta-alpha;
    for(size_t i=0;i<=np;++i)
    {
        const double phi = alpha + (i*delta)/np;
        const double x   = a*cos(phi);
        const double y   = b*sin(phi);
        P.add(phi,V2D(x,y));
    }

    P.add(10,V2D(2,-0.1));
    P.add(11,V2D(2.2,0));
    P.add(12,V2D(2,0.1));




    YACK_CHECK(P.scale());
    std::cerr << "P.lower  = " << P.lower() << std::endl;
    std::cerr << "P.upper  = " << P.upper() << std::endl;
    std::cerr << "P.period = " << P.period  << std::endl;

    save_spline("init.dat",P);
    {
        ios::ocstream fp("ctrl.dat");
        for(size_t i=1;i<=P.size();++i)
        {
            fp("%g %g\n", P.y[i].x, P.y[i].y );
        }
        fp("%g %g\n", P.y[1].x, P.y[1].y );
    }

}
YACK_UDONE()
