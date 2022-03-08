#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"
#include "yack/math/numeric.hpp"


#include "yack/math/triplet.hpp"
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


        double components:: compute_extent(const readable<double> &C0,
                                           const readable<double> &Cs) const throw()
        {
            YACK_CHEM_PRINTLN("//   <Xi>");
            double sum = 0;
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                const int        n = c.coef;
                const species   &s = *c;
                const size_t     j = *s;
                const double    dC = Cs[j] - C0[j];
                const double    xi = dC/n;
                YACK_CHEM_PRINTLN("//   " << std::setw(14) << xi << " @" << s.name);
                sum += xi;
            }
            const double ans = sum/size();
            YACK_CHEM_PRINTLN("//   <Xi=" << ans << "/>");
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

            ios::ocstream::overwrite("extent.dat");

            YACK_CHEM_PRINTLN("// <extent>");
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
                YACK_CHEM_PRINTLN("//   <already@0>");
                return 0;
            }

            assert( fabs(compute_extent(C0,Cs))<=0 );
            double xiOld = 0;
            size_t cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // f.b = f0 is computed with its sign
            //
            //------------------------------------------------------------------
            assert(__zero__!=s0);
            const limits &lim = private_limits(Cs);
            x.b = 0;
            YACK_CHEM_PRINTLN("//   " << lim);

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


            //std::cerr << "    x=" << x << std::endl;
            //std::cerr << "    f=" << f << std::endl;

            if(x.a>=x.c)
            {
                std::cerr << "DO SOMETHING" << std::endl;
                exit(1);
            }

            // find xi and move Cs
            scaled_call  G  = { *this, K, Cs, x, 0 };
            const double xi = G.solve(f);
            YACK_CHEM_PRINTLN("//   xi=" << xi);
            move(Cs,xi);

            f.b = mass_action(K,Cs);
            s0  = __sign::of(f.b);
            const double xiNew = compute_extent(C0,Cs);
            ios::ocstream::echo("extent.dat", "%.15g %.15g %.15g\n", double(cycle), xiNew, xi);
            if( __zero__ == s0 )
            {
                YACK_CHEM_PRINTLN("//   exact zero mass action");
                return xiNew;
            }

            YACK_CHEM_PRINTLN("//    Xi: " << xiOld << " -> " << xiNew);

            const double dXi = fabs(xiNew-xiOld);
            std::cerr << "dXi=" << fabs(xiNew-xiOld) << "/xi=" << xiOld << std::endl;
            if( dXi <= numeric<double>::ftol * fabs(xiOld) )
            {
                YACK_CHEM_PRINTLN("//   extent convergence");
                return compute_extent(C0,Cs);
            }

            if(cycle>=20)
            {
                std::cerr << "exit for ";
                display(std::cerr) << std::endl;
                exit(1);
            }

            xiOld = xiNew;
            goto CYCLE;

            

        }

    }

}
