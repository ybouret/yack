
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace math
    {
        template <>
        void minimize:: move<real_t>:: one_step(triplet<real_t>       &x,
                                                triplet<real_t>       &f,
                                                real_function<real_t> &F)
        {
            static const real_t tiny(1e-30);
            static const real_t one(1);
            assert( x.is_increasing() );
            assert( f.a>=f.b );
            assert( f.c>=f.b );
            assert(tiny>0);

            const real_t w     = fabs(x.c-x.a);
            const real_t beta  = (x.b<=x.a) ? 0 : (x.c<=x.b) ? 1 : (x.b-x.a)/w;
            const real_t d_a   = fabs(f.a-f.b)+tiny; assert(d_a>0);
            const real_t d_c   = fabs(f.c-f.b)+tiny; assert(d_c>0);
            const real_t beta2 = beta*beta;
            const real_t num   = (one-beta2) * d_a + beta2 * d_c;
            const real_t den   = (one-beta)  * d_a + beta  * d_c;
            const real_t u     = clamp<real_t>(x.a,x.a+num/(den+den)*w,x.c);

            std::cerr << "x=" << x << std::endl;
            std::cerr << "f=" << f << std::endl;
            std::cerr << "u=" << u << std::endl;

            {
                ios::ocstream fp("min-fcn.dat");
                for(real_t v=0;v<=1.0;v += 0.01)
                {
                    const real_t xx = x.a + v * w;
                    fp("%g %g\n", double(xx), double(F(xx)));
                }
            }

            {
                ios::ocstream fp("min-dat.dat");
                fp("%g %g\n", double(x.a), double(f.a));
                fp("%g %g\n", double(x.b), double(f.b));
                fp("%g %g\n", double(x.c), double(f.c));

            }
        }

    }
}

