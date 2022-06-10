
#include "yack/math/opt/locate.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

static inline
void build_para(const triplet<double> &x, const triplet<double> &f, const char *root)
{
    const double w    = x.c-x.a;
    const double beta = (x.b-x.a)/w;
    std::cerr << "beta=" << beta << std::endl;
    const double den = beta*(beta-1.0);
    const double d_a = f.a - f.b;
    const double d_c = f.c - f.b;
    const double lam = (squared(1.0-beta)*d_a - squared(beta) * d_c)/den;
    const double mu  = ((beta-1.0)*d_a - beta*d_c)/den;

    {
        string fn = root; fn += "-data.dat";
        ios::ocstream fp(fn);
        fp("%g %g\n",x.a,f.a);
        fp("%g %g\n",x.b,f.b);
        fp("%g %g\n",x.c,f.c);
    }

    {
        string fn = root; fn += "-para.dat";
        ios::ocstream fp(fn);
        static const size_t NP = 100;
        for(size_t i=0;i<=NP;++i)
        {
            const double u = double(i)/NP;
            const double F = f.b + lam*(u-beta) + mu * squared(u-beta);
            fp("%g %g\n", x.a + u * w, F);
        }
    }

}

YACK_UTEST(parabolic)
{

    triplet<double> x = { 0, 0.1, 0.7 };
    triplet<double> f = { 1, 0.3, 0.4 };

    build_para(x,f,"sample");

    f.c = f.a;
    build_para(x,f,"mid1");

    x.b = 0.6;
    build_para(x,f,"mid2");

    x.b = 0.02;
    build_para(x,f,"mid3");

    x.b = 0.2;
    f.a = f.b;
    build_para(x,f,"lhs");





}
YACK_UDONE()

