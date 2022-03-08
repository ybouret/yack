
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


            struct direct_call
            {
                const components       &self;
                const double            K;
                const readable<double> &C;

                inline double operator()(const double xi) const throw()
                {
                    return self.mass_action(K,C,xi);
                }
            };

            struct scaled_call
            {
                const components       &self;
                const double            K;
                const readable<double> &C;
                const triplet<double>  &X;
                mutable double          last_xi;

                inline double computeXi(const double omega) const throw()
                {
                    return ( last_xi = clamp(X.a,(1.0-omega) * X.a + omega * X.c,X.c) );
                }

                inline double operator()(const double omega) const throw()
                {
                    return self.mass_action(K,C,computeXi(omega));
                }

                inline bool update(triplet<double>    &w,
                                   triplet<double>    &f) const throw()
                {
                    const scaled_call &G = *this;
                    switch( __sign::of(f.b = G(w.b=0.5*(w.a+w.c))))
                    {
                        case __zero__:
                            return true;

                        case positive:
                            f.a = f.b;
                            w.a = w.b;
                            break;

                        case negative:
                            f.c = f.b;
                            w.c = w.b;
                            break;
                    }
                    return false;
                }

                inline double  operator*() const throw() { return last_xi; }

            };



        }

#if 0
        static inline bool same_magnitude( const double X, const double Y ) throw()
        {
            static const double p  = 2;
            static const double lo = pow(10,-p);
            static const double hi = pow(10,p);
            const double AX = fabs(X);
            const double AY = fabs(Y);
            return ( (lo * AY <= AX) && (AX <= hi * AY) ) || ( (lo * AX <= AY) && (AY <= hi * AX) );
        }
#endif

        double components:: extent(const double            K,
                                   const readable<double> &C) const
        {

            assert(K>0);
            assert(are_valid(C));

            // initialize by computing limits and f0=f.b=mass.action(K.C)@0
            const limits      &lim  = private_limits(C);
            triplet<double>    x    = { 0, 0, 0 };
            triplet<double>    f    = { 0, mass_action(K,C), 0 };
            triplet<sign_type> s    = { __zero__, __sign::of(f.b), __zero__ };
            std::cerr << "f0=" << f.b << " @" << x.b << std::endl;
            std::cerr <<  lim  << std::endl;


            if(s.b==__zero__)
            {
                // special case
                std::cerr << "early return @0" << std::endl;
                return 0;
            }
            else
            {

                // generic case
                switch(lim.type)
                {
                        //------------------------------------------------------
                    case limited_by_none:
                        //------------------------------------------------------
                        throw exception("invalid (empty) components");

                        //------------------------------------------------------
                    case limited_by_prod:
                        //------------------------------------------------------
                        assert(nu_r==0);
                        assert(nu_p>0);
                        assert(d_nu==nu_p);
                        if(positive==s.b)
                        {
                            const direct_call  F    = { *this, K, C };
                            x.a = x.b; // =0
                            f.a = f.b; // @0

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
                            x.c = x.b;  // =0
                            f.c = f.b;  // @0
                        }

                        break;

                        //------------------------------------------------------
                    case limited_by_reac:
                        //------------------------------------------------------
                        assert(nu_r>0);
                        assert(nu_p==0);
                        assert(d_nu==-nu_r);
                        if(positive==s.b)
                        {
                            x.a = x.b; // =0
                            f.a = f.b; // @0
                            x.c = lim.reac_extent(); assert(x.c>=0);
                            f.c = -1;                assert(f.c<0);
                        }
                        else
                        {
                            const direct_call  F    = { *this, K, C };
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

                        //------------------------------------------------------
                    case limited_by_both:
                        //------------------------------------------------------
                        assert(nu_r>0);
                        assert(nu_p>0);
                        if(positive==s.b)
                        {
                            x.c = lim.reac_extent();              assert(x.c>=0);
                            f.c = - (prod.mass_action(1,C,x.c));  assert(f.c<0);

                            x.a = x.b;  // =0
                            f.a = f.b;  // @0
                        }
                        else
                        {
                            assert(negative==s.b);
                            x.a = lim.prod_extent();            assert(x.a<=0);
                            f.a = (reac.mass_action(K,C,-x.a)); assert(f.a>0);
                            x.c = x.b;  // =0
                            f.c = f.b;  // @0
                        }
                        break;
                }
                std::cerr << "x=" << x << "; f=" << f << std::endl;

                if(x.a>=x.c)
                {
                    std::cerr << "blocked @0" << std::endl;
                    return 0;
                }

                const scaled_call G = { *this, K, C, x, 0};

                {
                    ios::ocstream fp("zext.dat");
                    const int N = 10000;
                    for(int i=0;i<=N;++i)
                    {
                        const double xx =  double(i)/N;
                        fp("%.15g %.15g\n",xx,G(xx));
                    }
                }


                triplet<double> w = { 0, 0, 1 };


                size_t cycle = 0;
                double width = 1.0;
            CYCLE:
                ++cycle;
                if(G.update(w,f))
                {
                    return *G;
                }
                std::cerr << "w=" << w << ", f=" << f << std::endl;
                const double new_width = fabs(w.c-w.a);
                if(new_width>=width)
                {
                    exit(1);
                }

                goto CYCLE;



            }

            
        }

    }

}
