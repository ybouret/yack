
#include "yack/math/opt/optimize.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/lua++/function.hpp"

using namespace yack;
using namespace math;

template <typename T> static inline
T F(T x)
{
    return 1.27-cos(3*x-0.1);
}

#if 0
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
#endif



YACK_UTEST(parabolic)
{
    locate::verbose   = true;
    optimize::verbose = true;

    triplet<double> x = {0,-1,0.7 };

    Lua::VM vm = new Lua::State;
    for(int i=1;i<argc;++i)
    {
        vm->dostring(argv[i]);
    }

    vm->getglobal("a");
    if( vm->type(-1) == LUA_TNUMBER )
    {
        x.a = vm->to<lua_Number>(-1);
    }

    vm->getglobal("c");
    if( vm->type(-1) == LUA_TNUMBER )
    {
        x.c = vm->to<lua_Number>(-1);
    }

    vm->getglobal("F");
    if( vm->type(-1) == LUA_TFUNCTION )
    {
        Lua::Function<double> FF(vm,"F");
        triplet<double> f = { FF(x.a), -1, FF(x.c) };
        optimize::run_for(FF,x,f,optimize::inside);
    }
    else
    {
        triplet<double> f = { F(x.a), -1, F(x.c) };
        optimize::run_for(F<double>,x,f,optimize::inside);
    }
    
    // parabolic a=-0.3 c=0.4 "function F(t) return 2+math.sin(t)^2; end"
}
YACK_UDONE()

