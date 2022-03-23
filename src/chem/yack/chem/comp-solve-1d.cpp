
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

                // mass_action(K,Cs,computeXi(omega))
                inline double operator()(const double omega) throw()
                {
                    return self.mass_action(K,C,computeXi(omega));
                }


                // single state update, return true on 'exact' zero
                inline bool update(triplet<double>    &w,
                                   triplet<double>    &f)  throw()
                {
                    scaled_call &G = *this;
                    assert(f.a>=0);
                    assert(f.c<=0);

                    w.b=0.5*(w.a+w.c);
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
                        //YACK_CHEM_PRINTLN("// exact omega");
                        return last_xi;
                    }

                    double widthOld = fabs(omega.c-omega.a);
                    while(true)
                    {
                        if(update(omega,f))
                        {
                            //YACK_CHEM_PRINTLN("// exact omega");
                            return last_xi;
                        }

                        const double widthNew = fabs(omega.c-omega.a);
                        if(widthNew>=widthOld)
                        {
                            //YACK_CHEM_PRINTLN("// converged on width");
                            return last_xi;
                        }

                        //omegaOld=omegaNew;
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
            assert(K>0);
            assert(are_valid(C0));
            assert(Cs.size()>=C0.size());



            //------------------------------------------------------------------
            //
            // initialize Cs
            //
            //------------------------------------------------------------------
            tao::v1::load(Cs,C0);               //!< workspace is Cs
            double    f0 =  mass_action(K,Cs);  //!< initial mass action
            sign_type s0 = __sign::of(f0);      //!< sign of mass action
            if( __zero__ == s0 )
            {
                // exact solution already
                return 0;
            }

            double oldXi = 0;       //!< old found extent
            bool   first = true;    //!< first cycle
            size_t cycle = 0;       //!< counter

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // compute limits , using f.b = f0 is computed with its sign
            //
            //------------------------------------------------------------------
            assert(__zero__      != s0);
            assert(__sign::of(f0)== s0);
            const limits   &lim = private_limits(Cs,0);
            triplet<double> x   = { 0, 0,  0 }; //! exact range
            triplet<double> f   = { 0, f0, 0 }; //! mass action


            switch(lim.type)
            {
                    //----------------------------------------------------------
                    //
                case limited_by_none:
                    //
                    //----------------------------------------------------------
                    throw exception("invalid (empty) components");

                    //----------------------------------------------------------
                    //
                case limited_by_prod:
                    //
                    //----------------------------------------------------------
                    assert(nu_r==0);
                    assert(nu_p>0);
                    assert(d_nu==nu_p);
                    if(positive==s0)
                    {
                        x.a = x.b; // = 0
                        f.a = f.b; // > 0

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

                    //----------------------------------------------------------
                    //
                case limited_by_reac:
                    //
                    //----------------------------------------------------------
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

                    //----------------------------------------------------------
                    //
                case limited_by_both:
                    //
                    //----------------------------------------------------------
                    assert(nu_r>0);
                    assert(nu_p>0);
                    if(positive==s0)
                    {
                        x.c = lim.reac_extent();               assert(x.c>=0);
                        f.c = - (prod.mass_action(1,Cs,x.c));  assert(f.c<0);

                        x.a = x.b;  // =0
                        f.a = f.b;  // >0
                    }
                    else
                    {
                        assert(negative==s0);
                        x.a = lim.prod_extent();             assert(x.a<=0);
                        f.a = (reac.mass_action(K,Cs,-x.a)); assert(f.a>0);
                        x.c = x.b;  // =0
                        f.c = f.b;  // <0
                    }
                    break;

                default:
                    throw exception("not implemented yet");
            }

            if(x.a>=x.c)
            {
                //YACK_CHEM_PRINTLN("//   numerical empty search");
                return deduce(C0,Cs);
            }

            //------------------------------------------------------------------
            //
            // solve with local xi in [x.a:x.c]
            //
            //------------------------------------------------------------------
            scaled_call  G     = { *this, K, Cs, x, 0};
            const double xiNew = G.solve(f);

            if(first)
            {
                //YACK_CHEM_PRINTLN("//   xi=" << xiNew << " (first)");
                first = false;
            }
            else
            {
                //YACK_CHEM_PRINTLN("//   xi=" << xiNew << " / " << oldXi);
                if(fabs(xiNew)>=fabs(oldXi))
                {
                    // reached numerical limit at previous step, do not move more!
                    //YACK_CHEM_PRINTLN("// reached numerical limit");
                    return deduce(C0,Cs);
                }
            }

            //------------------------------------------------------------------
            //
            // update workspace
            //
            //------------------------------------------------------------------
            move(Cs,xiNew);
            f0    =  mass_action(K,Cs);
            s0    = __sign::of(f0);
            if(__zero__==s0)
            {
                //YACK_CHEM_PRINTLN("// exact zero mass action");
                return deduce(C0,Cs);
            }

            //------------------------------------------------------------------
            //
            // ready for new cycle
            //
            //------------------------------------------------------------------
            oldXi = xiNew;
            goto CYCLE;

        }

    }

}


