
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"
#include "yack/math/root/zbis.hpp"
#include "yack/math/root/zrid.hpp"

#include "yack/exception.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        namespace
        {
            struct eqz
            {
                const components       &self;
                const double            K;
                const readable<double> &C;

                inline double operator()(const double xi) const throw()
                {
                    return self.mass_action(K,C,xi);
                }
            };
        }

        double components:: extent(const double            K,
                                   const readable<double> &C) const
        {

            const limits &lim = private_limits(C);
            //zrid<double>  zfx;
            zbis<double>  zfx;
            zfx.verbose = true;
            std::cerr << lim << std::endl;
            triplet<double> x = { 0, 0, 0 };
            triplet<double> f = { 0, 0, 0 };
            const eqz       F = { *this, K, C };

            switch(lim.type)
            {
                case limited_by_both:
                    x.a = -lim.prod->xi; f.a =  reac.mass_action(K,C,-x.a); assert(f.a>=0);
                    x.c =  lim.reac->xi; f.c = -prod.mass_action(1,C,x.c);  assert(f.c<=0);
                    std::cerr << "x=" << x << ", f=" << f << std::endl;

                {
                    ios::ocstream fp("both.dat");
                    const int N = 1000;
                    for(int i=0;i<=N;++i)
                    {
                        const double xx = x.a+ (i*(x.c-x.a))/N;
                        fp("%.15g %.15g\n",xx,F(xx));
                    }
                }

                    if(!zfx(F,x,f) )
                    {
                        throw exception("cannot solve");
                    }



                    std::cerr << "xi=" << x.b << std::endl;
                    std::cerr << "ma=" << f.b << std::endl;
                    break;

                case limited_by_prod:
                    x.a = -lim.prod->xi; f.a =  reac.mass_action(K,C,-x.a); assert(f.a>=0);
                    x.c = max_of( pow(K,sexp), maximum(C) );
                    f.c = F(x.c);
                    while(f.c>0)
                    {
                        f.c = F(x.c*=2);
                    }
                    std::cerr << "x=" << x << ", f=" << f << std::endl;

                    exit(1);
                    break;

                default:
                    throw exception("not implemented");
            }


            return x.b;

        }

    }

}
