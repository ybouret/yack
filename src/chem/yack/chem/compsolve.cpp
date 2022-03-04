
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

            assert(K>0);
            assert(are_valid(C));




            const limits   &lim  = private_limits(C);
            triplet<double> x    = { 0, 0, 0 };
            triplet<double> f    = { 0, 0, 0 };
            const eqz       F    = { *this, K, C };
            std::cerr << lim << std::endl;

            switch(lim.type)
            {
                case limited_by_none:
                    throw exception("invalid components");

                case limited_by_prod:
                    assert(nu_r==0);
                    assert(nu_p>0);
                    assert(d_nu==nu_p);
                    x.a = lim.prod_extent(); f.a =  K; assert(f.a>0);
                    x.c = pow(K,sexp);
                    f.c = F(x.c);
                    while(f.c>0)
                    {
                        f.c = F(x.c*=2);
                    }
                    break;


                case limited_by_reac:
                    assert(nu_r>0);
                    assert(nu_p==0);
                    assert(d_nu==-nu_r);
                    x.c = lim.reac_extent(); f.c = -1;
                    x.a = -pow(K,sexp);
                    f.a = F(x.a);
                    while(f.a<0)
                    {
                        f.a = F(x.a*=2);
                    }
                    break;

                default:
                    throw exception("not implemented");

            }

            std::cerr << "x=" << x << ", f=" << f << std::endl;

            {
                ios::ocstream fp("zext.dat");
                const int N = 1000;
                for(int i=0;i<=N;++i)
                {
                    const double xx = x.a+ (i*(x.c-x.a))/N;
                    fp("%.15g %.15g\n",xx,F(xx));
                }
            }

            exit(1);

            return 0;

#if 0
            const double  mexp = nu_p > 0 ? 1.0/nu_p : 1.0;
            //zrid<double>  zfx;
            zbis<double>  zfx;
            zfx.verbose = true;
            triplet<double> x = { 0, 0, 0 };
            triplet<double> f = { 0, 0, 0 };
            const eqz       F = { *this, K, C, mexp };

            switch(lim.type)
            {
                case limited_by_both:
                    x.a = -lim.prod->xi; f.a =   (reac.mass_action(K,C,-x.a)); assert(f.a>=0);
                    x.c =  lim.reac->xi; f.c = - (prod.mass_action(1,C,x.c));  assert(f.c<=0);
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
#endif
        }

    }

}
