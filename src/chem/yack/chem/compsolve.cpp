
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"

#if 0
#include "yack/math/root/zbis.hpp"
#include "yack/math/root/zrid.hpp"
#include "yack/math/root/zsec.hpp"
#endif

#include "yack/math/triplet.hpp"
//#include "yack/math/real-function.hpp"
#include "yack/signs.hpp"

#include "yack/exception.hpp"
#include <cmath>

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

            const limits      &lim  = private_limits(C);
            const eqz          F    = { *this, K, C };
            triplet<double>    x    = { 0, 0, 0 };
            triplet<double>    f    = { 0, mass_action(K,C), 0 };
            triplet<sign_type> s    = { __zero__, __sign::of(f.b), __zero__ };

            std::cerr << "f0=" << f.b << " @" << x.b << std::endl;
            std::cerr <<  lim  << std::endl;

            if(s.b==__zero__)
            {
                std::cerr << "early return @0" << std::endl;
                return 0;
            }
            else
            {

                switch(lim.type)
                {
                    case limited_by_none: throw exception("invalid (empty) components");


                    case limited_by_prod:
                        assert(nu_r==0);
                        assert(nu_p>0);
                        assert(d_nu==nu_p);
                        if(positive==s.b)
                        {
                            x.a = x.b;
                            f.a = f.b;

                            x.c = pow(K,sexp);
                            f.c = F(x.c); assert(x.c>0);
                            while(f.c>=0)
                            {
                                f.c = F(x.c*=2);
                            }
                            assert(f.c<0);
                        }
                        else
                        {
                            assert(negative==s.b);
                            x.a = lim.prod_extent();  assert(x.a<=0);
                            f.a = K;                  assert(f.a>0);
                            x.c = x.b;
                            f.c = f.b;
                        }

                        break;

                    case limited_by_reac:
                        assert(nu_r>0);
                        assert(nu_p==0);
                        assert(d_nu==-nu_r);
                        if(positive==s.b)
                        {
                            x.a = x.b;
                            f.a = f.b;
                            x.c = lim.reac_extent(); assert(x.c>=0);
                            f.c = -1;                assert(f.c<0);
                        }
                        else
                        {
                            assert(negative==s.b);
                            x.c = x.b;
                            f.c = f.b;
                            x.a = -pow(K,sexp);
                            f.a = F(x.a);
                            while(f.a<=0)
                            {
                                f.a = F(x.a*=2);
                            }
                            assert(x.a<0);
                            assert(f.a>0);
                        }
                        
                        break;


                    case limited_by_both:
                        assert(nu_r>0);
                        assert(nu_p>0);
                        if(positive==s.b)
                        {
                            x.c = lim.reac_extent();              assert(x.c>=0);
                            f.c = - (prod.mass_action(1,C,x.c));  assert(f.c<0);
                            x.a = x.b;
                            f.a = f.b;
                        }
                        else
                        {
                            assert(negative==s.b);
                            x.a = lim.prod_extent();            assert(x.a<=0);
                            f.a = (reac.mass_action(K,C,-x.a)); assert(f.a>0);
                            x.c = x.b;
                            f.c = f.b;
                        }
                        break;
                }
                std::cerr << "x=" << x << "; f=" << f << std::endl;


                {
                    ios::ocstream fp("zext.dat");
                    const int N = 10000;
                    for(int i=0;i<=N;++i)
                    {
                        const double xx = x.a+ (i*(x.c-x.a))/N;
                        fp("%.15g %.15g\n",xx,F(xx));
                    }
                }

                assert( __sign::of(f.a) == positive );
                assert( __sign::of(f.c) == negative );

                if(x.a>=x.c)
                {
                    std::cerr << "blocked @0" << std::endl;
                    return 0;
                }

                size_t cycle = 0;
            CYCLE:
                ++cycle;
                x.b = 0.5*(x.a+x.c);
                f.b = F(x.b);
                switch( s.b = __sign::of(f.b) )
                {
                    case __zero__: return x.b; // early numerical return
                    case negative: f.c = f.b; x.c = x.b; break;
                    case positive: f.a = f.b; x.a = x.b; break;
                }
                assert( __sign::of(f.a) == positive );
                assert( __sign::of(f.c) == negative );
                std::cerr << "x=" << x << "; f=" << f << " // @cycle=" << cycle << std::endl;
                if(cycle>=12)
                {
                    {
                        ios::ocstream fp("zext.dat");
                        const int N = 10000;
                        for(int i=0;i<=N;++i)
                        {
                            const double xx = x.a+ (i*(x.c-x.a))/N;
                            fp("%.15g %.15g\n",xx,F(xx));
                        }
                    }
                    exit(1);
                }
                goto CYCLE;

                return 0;
            }

            
        }

    }

}
