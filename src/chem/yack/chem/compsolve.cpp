
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"
#include "yack/math/numeric.hpp"

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

                inline double solve(triplet<double> &f) const throw()
                {
                    triplet<double> omega  = {0,0,1};

                    if(update(omega,f))
                    {
                        std::cerr << "exact omega" << std::endl;
                        return last_xi;
                    }

                    double omegaOld = omega.b;
                    double widthOld = fabs(omega.c-omega.a);
                    while(true)
                    {
                        if(update(omega,f))
                        {
                            std::cerr << "exact omega" << std::endl;
                            return last_xi;
                        }
                        const double omegaNew = omega.b;
                        if( fabs(omegaNew-omegaOld) <= numeric<double>::ftol * fabs(omegaOld) )
                        {
                            std::cerr << "converged on omega" << std::endl;
                            return last_xi;
                        }
                        const double widthNew = fabs(omega.c-omega.a);
                        if(widthNew>=widthOld)
                        {
                            std::cerr << "converged on width" << std::endl;
                            return last_xi;
                        }
                        omegaOld=omegaNew;
                        widthOld=widthNew;
                    }

                }

            };

        }


        double components:: compute_extent(const readable<double> &C0,
                                           const readable<double> &Cs) const throw()
        {
            std::cerr << "Computing Extent" << std::endl;
            double sum = 0;
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                const int        n = c.coef;
                const species   &s = *c;
                const size_t     j = *s;
                const double    dC = Cs[j] - C0[j];
                const double    xi = dC/n;
                std::cerr << s.name << " : " << xi << std::endl;
                sum += xi;
            }
            const double ans = sum/size();
            std::cerr << "Xi=" << ans << std::endl;
            return ans;
        }

        double components:: extent(const double            K,
                                   const readable<double> &C0,
                                   writable<double>       &Cs) const
        {

            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(K>0);
            assert(are_valid(C0));
            assert(Cs.size()>=C0.size());

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t j = *****node;
                Cs[j] = C0[j];
            }
            triplet<double>    x  = { 0, 0, 0 };
            triplet<double>    f  = { 0, mass_action(K,Cs), 0 };
            sign_type          s0 = __sign::of(f.b);

            if( __zero__ == s0 )
            {
                std::cerr << "already @0" << std::endl;
                return 0;
            }

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // f.b = f0 is computed with its sign
            //
            //------------------------------------------------------------------
            assert(__zero__!=s0);
            const limits &lim = private_limits(Cs); std::cerr << lim << std::endl;
            x.b = 0;


            //------------------------------------------------------------------
            //
            // find a bracketing interval
            //
            //------------------------------------------------------------------
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
                    if(positive==s0)
                    {
                        const direct_call  F = { *this, K, Cs };
                        x.a = x.b; // =0
                        f.a = f.b; // >0

                        x.c = pow(K,sexp);
                        f.c = F(x.c); assert(x.c>0);
                        while(f.c>=0) f.c = F(x.c*=2);
                        assert(f.c<0);
                    }
                    else
                    {
                        assert(negative==s0);
                        x.a = lim.prod_extent();  assert(x.a<=0);
                        f.a = K;                  assert(f.a>0);
                        x.c = x.b;  // =0
                        f.c = f.b;  // <0
                    }
                    break;

                    //------------------------------------------------------
                case limited_by_reac:
                    //------------------------------------------------------
                    assert(nu_r>0);
                    assert(nu_p==0);
                    assert(d_nu==-nu_r);
                    if(positive==s0)
                    {
                        x.a = x.b; // =0
                        f.a = f.b; // @0
                        x.c = lim.reac_extent(); assert(x.c>=0);
                        f.c = -1;                assert(f.c<0);
                    }
                    else
                    {
                        const direct_call  F    = { *this, K, Cs };
                        assert(negative==s0);
                        x.c = x.b;
                        f.c = f.b;
                        x.a = -pow(K,sexp);
                        f.a = F(x.a);
                        while(f.a<=0) f.a = F(x.a*=2);
                        assert(x.a<0);
                        assert(f.a>0);
                    }
                    break;

                    //------------------------------------------------------
                case limited_by_both:
                    //------------------------------------------------------
                    assert(nu_r>0);
                    assert(nu_p>0);
                    if(positive==s0)
                    {
                        x.c = lim.reac_extent();               assert(x.c>=0);
                        f.c = - (prod.mass_action(1,Cs,x.c));  assert(f.c<0);

                        x.a = x.b;  // =0
                        f.a = f.b;  // @0
                    }
                    else
                    {
                        assert(negative==s0);
                        x.a = lim.prod_extent();             assert(x.a<=0);
                        f.a = (reac.mass_action(K,Cs,-x.a)); assert(f.a>0);
                        x.c = x.b;  // =0
                        f.c = f.b;  // @0
                    }
                    break;

                default:
                    throw exception("not implemented yet");
            }

            std::cerr << "    x=" << x << std::endl;
            std::cerr << "    f=" << f << std::endl;

            if(x.a>=x.c)
            {
                std::cerr << "DO SOMETHING" << std::endl;
                exit(1);
            }

            {
                const scaled_call G     = { *this, K, Cs, x, 0 };
                const double      xi    = G.solve(f);
                std::cerr << "xi=" << xi << std::endl;
                move(Cs,xi);
            }

            f.b = mass_action(K,Cs);
            s0  = __sign::of(f.b);
            if( __zero__ == s0 )
            {
                std::cerr << "exact zero mass action" << std::endl;
                return compute_extent(C0,Cs);
            }

            if(cycle>=4)
            {
                exit(1);
            }

            goto CYCLE;


#if 0
            if(__zero__==s0)
            {
                std::cerr << "early return @0" << std::endl;
                return compute_extent(C0,Cs);
            }
            else
            {
                triplet<double>    x    = { 0, 0,  0 };
                triplet<double>    f    = { 0, f0, 0 };

                //--------------------------------------------------------------
                //
                // bracket zero in [x.a:x.c]
                //
                //--------------------------------------------------------------
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
                        if(positive==s0)
                        {
                            const direct_call  F = { *this, K, Cs };
                            x.a = x.b; // =0
                            f.a = f.b; // >0

                            x.c = pow(K,sexp);
                            f.c = F(x.c); assert(x.c>0);
                            while(f.c>=0) f.c = F(x.c*=2);
                            assert(f.c<0);
                        }
                        else
                        {
                            assert(negative==s0);
                            x.a = lim.prod_extent();  assert(x.a<=0);
                            f.a = K;                  assert(f.a>0);
                            x.c = x.b;  // =0
                            f.c = f.b;  // <0
                        }

                        break;

                        //------------------------------------------------------
                    case limited_by_reac:
                        //------------------------------------------------------
                        assert(nu_r>0);
                        assert(nu_p==0);
                        assert(d_nu==-nu_r);
                        if(positive==s0)
                        {
                            x.a = x.b; // =0
                            f.a = f.b; // @0
                            x.c = lim.reac_extent(); assert(x.c>=0);
                            f.c = -1;                assert(f.c<0);
                        }
                        else
                        {
                            const direct_call  F    = { *this, K, Cs };
                            assert(negative==s0);
                            x.c = x.b;
                            f.c = f.b;
                            x.a = -pow(K,sexp);
                            f.a = F(x.a);
                            while(f.a<=0) f.a = F(x.a*=2);
                            assert(x.a<0);
                            assert(f.a>0);
                        }
                        break;

                        //------------------------------------------------------
                    case limited_by_both:
                        //------------------------------------------------------
                        assert(nu_r>0);
                        assert(nu_p>0);
                        if(positive==s0)
                        {
                            x.c = lim.reac_extent();               assert(x.c>=0);
                            f.c = - (prod.mass_action(1,Cs,x.c));  assert(f.c<0);

                            x.a = x.b;  // =0
                            f.a = f.b;  // @0
                        }
                        else
                        {
                            assert(negative==s0);
                            x.a = lim.prod_extent();             assert(x.a<=0);
                            f.a = (reac.mass_action(K,Cs,-x.a)); assert(f.a>0);
                            x.c = x.b;  // =0
                            f.c = f.b;  // @0
                        }
                        break;
                }
                std::cerr << "x=" << x << "; f=" << f << std::endl;

                if(x.a>=x.c)
                {
                    std::cerr << "blocked @0" << std::endl;
                    return compute_extent(C0,Cs);
                }

                if(false)
                {
                    ios::ocstream fp("zext.dat");
                    const unsigned N = 1000;
                    for(unsigned i=0;i<=N;++i)
                    {
                        const double w = double(i)/N;
                        const double xx = x.a * (1.0-w) + x.c*w;
                        fp("%g %g\n", xx, mass_action(K,Cs,xx) );
                    }
                }

                //--------------------------------------------------------------
                //
                // best guess : secant
                //
                //--------------------------------------------------------------
                static const double wmin = numeric<double>::ftol;
                static const double wmax = 1.0-wmin;
                if( fabs(f.a) < fabs(f.c) )
                {
                    const double omega = clamp(wmin,f.a/(f.a-f.c),wmax);
                    x.b = (1.0-omega) * x.a + omega * x.c;
                }
                else
                {
                    const double omega = clamp(wmin,(-f.c)/(f.a-f.c),wmax);
                    x.b = omega * x.a + (1.0-omega) * x.c;
                }



                std::cerr << "found " << x.b << " @cycle=" << cycle << std::endl;
                move(Cs,x.b);
                std::cerr << "Cs=" << Cs << std::endl;

                if(cycle>=8) exit(1);

                goto LOOP;
            }

#endif



        }

    }

}
