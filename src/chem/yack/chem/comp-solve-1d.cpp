
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

            struct MASolver
            {
                const components       &self;
                const double            K;
                const readable<double> &C;


                // call function: self.mass_action(K,C,xx)
                inline double operator()(const double xx) throw()
                {
                    return self.mass_action(K,C,xx);
                }


                // apply bisection reduction
                // with precomputed sign of f.b
                inline void b_update(triplet<double> &x,
                                     triplet<double> &f,
                                     const sign_type  s) throw()
                {
                    assert(__zero__       !=  s);
                    assert(__sign::of(f.b) == s);

                    assert(f.a>0);
                    assert(f.c<0);
                    assert(x.c>x.a);
                    assert(x.a<=x.b);
                    assert(x.b<=x.c);

                    if(positive==s)
                    {
                        f.a = f.b;
                        x.a = x.b;
                    }
                    else
                    {
                        assert(negative==s);
                        f.c = f.b;
                        x.c = x.b;
                    }
                }


#define YACK_CHEM_SANITY() \
assert(f.a>0);             \
assert(f.c<0);             \
assert(x.c>x.a)


                // try an extended step with a precomputed sign of f.b
                // and the Ridder's coefficient
                inline bool extended(triplet<double> &x,
                                     triplet<double> &f,
                                     const sign_type  s,
                                     const double     d) throw()
                {
                    assert(__zero__       !=  s);
                    assert(__sign::of(f.b) == s);
                    assert(d>fabs(f.b));
                    YACK_CHEM_SANITY();
                    assert(x.a<=x.b);
                    assert(x.b<=x.c);

                    {
                        const double xm = x.b + 0.5 * (x.c-x.a) * f.b/d;
                        if(positive==s)
                        {
                            //--------------------------------------------------
                            //
                            // f.b > 0
                            // trial between x.b and x.c
                            //
                            //--------------------------------------------------
                            if(xm<=x.b||xm>=x.c)
                            {
                                // underflow, apply bisection
                                f.a = f.b;
                                x.a = x.b;
                                YACK_CHEM_SANITY();
                            }
                            else
                            {
                                // check value
                                const double fm = (*this)(xm);
                                switch( __sign::of(fm) )
                                {
                                    case __zero__: // early return
                                        f.b = 0;
                                        x.b = xm;
                                        return true;

                                    case negative:
                                        f.a = f.b; x.a = x.b; // a->b (positive)
                                        f.c = fm;  x.c = xm;  // c->m (negative)
                                        YACK_CHEM_SANITY();
                                        break;

                                    case positive:
                                        f.a = fm; x.a = xm; // a->m (positive)
                                        YACK_CHEM_SANITY();
                                        break;
                                }
                            }
                        }
                        else
                        {
                            assert(negative==s);
                            //--------------------------------------------------
                            //
                            // f.b < 0
                            // trial between x.a and x.b
                            //
                            //--------------------------------------------------

                            if(xm<=x.a||xm>=x.b)
                            {
                                // underflow, apply bisection
                                f.c = f.b;
                                x.c = x.b;
                                YACK_CHEM_SANITY();
                            }
                            else
                            {
                                // evaluate
                                const double fm = (*this)(xm);
                                switch( __sign::of(fm) )
                                {
                                    case __zero__:
                                        f.b = 0;
                                        x.b = xm;
                                        return true; // early return

                                    case negative:
                                        f.c = fm; x.c = xm; //!< c->m, negative
                                        YACK_CHEM_SANITY();
                                        break;

                                    case positive:
                                        f.a = fm;  x.a = xm;   //!< a->m, positive
                                        f.c = f.b; x.c = x.b;  //!< c->b, negative
                                        YACK_CHEM_SANITY();
                                        break;
                                }
                            }
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // update reduced interval
                    //
                    //----------------------------------------------------------
                    if(fabs(f.a)<fabs(f.c))
                    {
                        x.b = x.a;
                        f.b = f.a;
                    }
                    else
                    {
                        x.b = x.c;
                        f.b = f.c;
                    }

                    return false;
                }


                // reduce interval by hybrid methods
                inline bool update(triplet<double> &x,
                                   triplet<double> &f) throw()
                {
                    assert(f.a>0);
                    assert(f.c<0);
                    assert(x.c>x.a);

                    //----------------------------------------------------------
                    //
                    // evaluate middle point
                    //
                    //----------------------------------------------------------
                    const sign_type s = __sign::of( f.b = (*this)(x.b=0.5*(x.a+x.c)));
                    if(__zero__==s)
                    {
                        //------------------------------------------------------
                        //
                        // early return
                        //
                        //------------------------------------------------------
                        return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // Initialize Ridder's reduction factor
                        //
                        //------------------------------------------------------
                        const double d = sqrt(f.b * f.b - f.a*f.c);
                        if(d>fabs(f.b))
                        {
                            //--------------------------------------------------
                            //
                            // extended step
                            //
                            //--------------------------------------------------
                            return extended(x,f,s,d);
                        }
                        else
                        {
                            //--------------------------------------------------
                            //
                            // bissection step due to numerical underflow
                            //
                            //--------------------------------------------------
                            b_update(x,f,s);
                            return false;
                        }
                    }
                }


                // maximum reduction of search interval
                inline double solve(triplet<double> &x,
                                    triplet<double> &f) throw()
                {

                    // initialize
                    if( update(x,f) )
                    {
                        return x.b;    // exact zero
                    }

                    // loop
                    double old_width = fabs(x.c-x.a);
                    while(true)
                    {
                        if( update(x,f) )
                        {
                            return x.b; // exact zero
                        }
                        const double new_width = fabs(x.c-x.a);

                        if(new_width>=old_width)
                        {
                            return x.b; // most accurate zero
                        }

                        old_width = new_width;
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

            double   oldXi = 0;       //!< old found extent
            bool     first = true;    //!< first cycle
            size_t   cycle = 0;       //!< counter
            MASolver mroot = { *this, K, Cs };

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
            triplet<double> x   = { 0, 0,  0 }; //! exact range, init x.b=0
            triplet<double> f   = { 0, f0, 0 }; //! mass action, init f.b=f(0)


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
                    break;


            }

            //------------------------------------------------------------------
            //
            // ready to solve
            //
            //------------------------------------------------------------------
            const double xiNew = mroot.solve(x,f);


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
                    // YACK_CHEM_PRINTLN("// reached numerical limit");
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


