#include "yack/chem/components.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/triplet.hpp"
#include "yack/exception.hpp"
#include <cmath>

namespace yack
{

    using namespace math;

    namespace chemical
    {

#define YACK_CHEM_RIDDER 1

        namespace
        {
            struct ZMA
            {
                const components       &S;
                const double            K;
                const readable<double> &C;
                mutable size_t          calls;

                inline double operator()(const double xi) const throw()
                {
                    ++calls;
                    return S.mass_action(K,C,xi);
                }


                static inline double _den(const triplet<double> &f) throw()
                {
                    assert(f.a>0);
                    assert(f.c<0);
                    return sqrt(f.b*f.b - f.a*f.c);
                }

                inline bool solve(triplet<double> &x,
                                  triplet<double> &f) const throw()
                {
                    assert(f.a>0);
                    assert(f.c<0);
                    const ZMA &F = *this;
                    double     w = fabs(x.c-x.a);

                TIGHTEN:
                    //----------------------------------------------------------
                    //
                    // take mid-point
                    //
                    //----------------------------------------------------------
                    assert(f.a>0);
                    assert(f.c<0);
                    x.b = clamp(x.a,0.5*(x.a+x.c),x.c);
                    f.b = F(x.b);

                    switch( __sign::of(f.b ))
                    {
                            //--------------------------------------------------
                            //
                        case __zero__: // early return
                            //
                            //--------------------------------------------------
                            return true;

                            //--------------------------------------------------
                            //
                        case negative: { // f.b is on the same side than f.c
                            //
                            //--------------------------------------------------
                            assert(f.b<0);
                            assert(f.c<0);

#if                         defined(YACK_CHEM_RIDDER)
                            const double den = _den(f);
#endif
                            // bisection c -> b
                            f.c = f.b;
                            x.c = x.b;

#if                         defined(YACK_CHEM_RIDDER)
                            if(den>fabs(f.b))
                            {
                                // Ridder's at left of x.b
                                const double x_r = clamp(x.a,x.b+0.5 * w * (f.b/den),x.b);
                                const double f_r = F(x_r);
                                switch( __sign::of(f_r) )
                                {
                                    case __zero__: // early return
                                        x.b = x_r;
                                        f.b = f_r;
                                        return true;

                                    case positive: // a -> r
                                        x.a =  x_r;
                                        f.a =  f_r;
                                        break;

                                    case negative: // c -> r, furthermore
                                        x.c = x_r;
                                        f.c = f_r;
                                        break;
                                }
                            }
#endif


                        } break;

                            //--------------------------------------------------
                            //
                        case positive:{  // f.b is on the same side than f.a
                            //
                            //--------------------------------------------------
                            assert(f.b>0);
                            assert(f.a>0);
#if                         defined(YACK_CHEM_RIDDER)
                            const double den = _den(f);
#endif
                            // bisection a -> b
                            f.a = f.b;
                            x.a = x.b;

#if                         defined(YACK_CHEM_RIDDER)
                            if(den>fabs(f.b))
                            {
                                // Ridder's at right of x.b
                                const double x_r = clamp(x.b,x.b+0.5 * w * (f.b/den),x.c);
                                const double f_r = F(x_r);
                                switch( __sign::of(f_r) )
                                {
                                    case __zero__: // early return
                                        x.b = x_r;
                                        f.b = f_r;
                                        return true;

                                    case positive: // a -> r, furthermore
                                        x.a =  x_r;
                                        f.a =  f_r;
                                        break;

                                    case negative: // c -> r
                                        x.c = x_r;
                                        f.c = f_r;
                                        break;
                                }
                            }
#endif

                        } break;
                    }
                    const double w_new = fabs(x.c-x.a);
                    if(w_new>=w)
                        return false; // no exactly 0, but convergence
                    w = w_new;
                    goto TIGHTEN;
                }
            };
        }
        

        outcome components:: brew1D(const double            K,
                                    const readable<double> &C0,
                                    writable<double>       &Cs,
                                    size_t                 *calls) const
        {
            //--------------------------------------------------------------
            //
            // initialize Cs
            //
            //--------------------------------------------------------------
            assert(Cs.size()>=C0.size());
            assert(are_valid(C0));
            iota::load(Cs,C0);



            if( reac.has_missing_actor(Cs) && prod.has_missing_actor(Cs) )
            {
                //--------------------------------------------------------------
                //
                // blocked!
                //
                //--------------------------------------------------------------
                return outcome(degenerated_extent,blocked_components,0);
            }


            //------------------------------------------------------------------
            //
            // running!
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            //
            // check initial state, with Cs=C0 => Xi=0
            //
            //------------------------------------------------------------------
            double    oldXi  = 0;
            bool      first  = true;
            const ZMA F      = { *this, K, Cs, 0};

        CYCLE:
            const double    f0 = mass_action(K,Cs);
            const sign_type s0 = __sign::of(f0);

            if(__zero__ == s0)
            {
                //--------------------------------------------------------------
                //
                // early return
                //
                //--------------------------------------------------------------
                const double xi = deduce(C0,Cs);
                return outcome(qualify_extent(xi,Cs),running_components,xi);
            }


            //------------------------------------------------------------------
            //
            // updating according to limiting extent(s)
            //
            //------------------------------------------------------------------
            const limits    &lim = private_limits(Cs,0);
            triplet<double>  x   = {0, 0, 0};
            triplet<double>  f   = {0, f0, 0};

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
                    assert( __sign::product_of(f.a,f.c) == negative );
                    assert( x.c>x.a );
                    assert(x.is_increasing());
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
                    assert( __sign::product_of(f.a,f.c) == negative );
                    assert( x.c>x.a );
                    assert(x.is_increasing());
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
                        x.c = lim.reac_extent();               assert(x.c>0);
                        f.c = - (prod.mass_action(1,Cs,x.c));  assert(f.c<0);

                        x.a = x.b;  // =0
                        f.a = f.b;  // >0
                        assert( __sign::product_of(f.a,f.c) == negative );
                        assert( x.c>x.a );
                    }
                    else
                    {
                        assert(negative==s0);
                        x.a = lim.prod_extent();             assert(x.a<0);
                        f.a = (reac.mass_action(K,Cs,-x.a)); assert(f.a>0);
                        x.c = x.b;  // =0
                        f.c = f.b;  // <0
                        assert( __sign::product_of(f.a,f.c) == negative );
                        assert( x.c>x.a );
                    }
                    assert(x.is_increasing());
                    break;
            }

            //__________________________________________________________________
            //
            // find new xi from local solving
            //__________________________________________________________________
            const bool   exact  = F.solve(x,f);
            const double newXi  = x.b;
            move(Cs,newXi);

            if(exact) goto SUCCESS;

            if(first)
            {
                // skip first cycle
                first = false;
            }
            else
            {
                // test for numerical limit
                if(fabs(newXi)>=fabs(oldXi)) goto SUCCESS;
            }

            oldXi = newXi;
            goto CYCLE;

        SUCCESS:
            if(calls) (*calls) += F.calls;
            const double xi = deduce(C0,Cs);
            return outcome(qualify_extent(xi,C0),running_components,xi);
        }



    }


}
