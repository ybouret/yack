
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/signs.hpp"
#include "yack/exception.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        namespace
        {
            struct scaled_call
            {
                const components       &self;
                const double            K;
                const readable<double> &C;
                const triplet<double>  &X;
                double                  last_xi;

                // compute Xi from original interval
                inline double computeXi(const double omega)  throw()
                {
                    return ( last_xi = clamp(X.a,(1.0-omega) * X.a + omega * X.c,X.c) );
                }

                // mass action from rescaled Xi
                inline double operator()(const double omega) throw()
                {
                    return self.mass_action(K,C,computeXi(omega));
                }


                // single state update
                inline bool update(triplet<double>    &w,
                                   triplet<double>    &f)  throw()
                {
                    scaled_call &G = *this;
                    assert(f.a>=0);
                    assert(f.c<=0);

#if 1
                    w.b=0.5*(w.a+w.c);
#else
                    static const double wmin = numeric<double>::ftol;
                    static const double wmax = 1.0-wmin;
                    if( fabs(f.a) < fabs(f.c) )
                    {
                        const double omega = clamp(wmin,f.a/(f.a-f.c),wmax);
                        w.b = (1.0-omega) * w.a + omega * w.c;
                    }
                    else
                    {
                        const double omega = clamp(wmin,(-f.c)/(f.a-f.c),wmax);
                        w.b = omega * w.a + (1.0-omega) * w.c;
                    }

#endif

                    YACK_CHEM_PRINTLN("// omega=" << w << ", f=" << f);
                    switch( __sign::of(f.b=G(w.b)) )
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


                inline double solve(triplet<double> &f) throw()
                {
                    triplet<double> omega  = {0,0,1};

                    if(update(omega,f))
                    {
                        YACK_CHEM_PRINTLN("// exact omega");
                        return last_xi;
                    }

                    double omegaOld = omega.b;
                    double widthOld = fabs(omega.c-omega.a);
                    while(true)
                    {
                        if(update(omega,f))
                        {
                            YACK_CHEM_PRINTLN("// exact omega");
                            return last_xi;
                        }
                        const double omegaNew = omega.b;
                        if( fabs(omegaNew-omegaOld) <= numeric<double>::ftol * fabs(omegaOld) )
                        {
                            YACK_CHEM_PRINTLN("// converged on omega");
                            return last_xi;
                        }
                        const double widthNew = fabs(omega.c-omega.a);
                        if(widthNew>=widthOld)
                        {
                            YACK_CHEM_PRINTLN("// converged on width");
                            return last_xi;
                        }
                        omegaOld=omegaNew;
                        widthOld=widthNew;
                    }

                }

            };
        }


        double components:: solve1D(const double            K,
                                    const readable<double> &C0,
                                    writable<double>       &Cs) const
        {

            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <extent>");
            
            assert(K>0);
            assert(are_valid(C0));
            assert(Cs.size()>=C0.size());



            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            tao::v1::load(Cs,C0);
            double             f0    =  mass_action(K,Cs);
            sign_type          s0    = __sign::of(f0);
            if( __zero__ == s0 )
            {
                YACK_CHEM_PRINTLN("//   <already@0>");
                return 0;
            }

            assert( fabs(deduce(C0,Cs))<=0 );


            double xiOld = 0;
            bool   first = true;
            size_t cycle = 0;

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // bracket, using f.b = f0 is computed with its sign
            //
            //------------------------------------------------------------------
            assert(__zero__!=s0);
            assert(__sign::of(f0)==s0);
            const limits   &lim = private_limits(Cs);
            triplet<double> x   = { 0, 0,  0 };
            triplet<double> f   = { 0, f0, 0 };

            YACK_CHEM_PRINTLN("//   " << lim);


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
                        x.a = x.b; // =0
                        f.a = f.b; // >0

                        x.c = pow(K,sexp);
                        f.c = mass_action(K,Cs,x.c); assert(x.c>0);
                        while(f.c>=0) f.c = mass_action(K,Cs,x.c*=2);
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
                        assert(negative==s0);
                        x.c = x.b;
                        f.c = f.b;
                        x.a = -pow(K,sexp);
                        f.a = mass_action(K,Cs,x.a);
                        while(f.a<=0) f.a = mass_action(K,Cs,x.a*=2);
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


            //std::cerr << "    x=" << x << std::endl;
            //std::cerr << "    f=" << f << std::endl;

            if(x.a>=x.c)
            {
                std::cerr << "DO SOMETHING" << std::endl;
                exit(1);
            }

            scaled_call G = { *this, K, Cs, x, 0 };
            if(false)
            {
                ios::ocstream fp("zext.dat");
                size_t N = 10000;
                for(size_t i=0;i<=N;++i)
                {
                    const double u = double(i)/N;
                    fp("%.15g %.15g\n",u,G(u));
                }
            }

            //------------------------------------------------------------------
            //
            // solve with local xi
            //
            //------------------------------------------------------------------
            const double xiNew = G.solve(f);
            YACK_CHEM_PRINTLN("//   xi=" << xiNew << " / " << xiOld);
            if(first)
            {
                first = false;
            }
            else
            {
                if(fabs(xiNew)>=fabs(xiOld))
                {
                    // reached numerical limit at previous step, do not movre
                    YACK_CHEM_PRINTLN("// reached numerical limit");
                    return deduce(C0,Cs);
                }
            }
            move(Cs,xiNew);

            f0    =  mass_action(K,Cs);
            s0    = __sign::of(f0);
            if(__zero__==s0)
            {
                YACK_CHEM_PRINTLN("// exact zero mass action");
                return deduce(C0,Cs);
            }
            xiOld = xiNew;


            goto CYCLE;

        }

    }

}


