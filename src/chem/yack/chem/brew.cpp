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


        namespace
        {
            struct ZMassAction
            {
                const components       &S;
                const double            K;
                const readable<double> &C;

                inline double operator()(const double xi) const throw()
                {
                    return S.mass_action(K,C,xi);
                }



                inline double solve(triplet<double> &x,
                                    triplet<double> &f) const throw()
                {
                    assert(f.a>0);
                    assert(f.c<0);
                    assert(x.is_increasing());
                    const ZMassAction &F = *this;
                    double             w = fabs(x.c-x.a);

                    // take mid-point
                    f.b = F( x.b = clamp(x.a,0.5*(x.a+x.c),x.c) );

                    // Ridder's Delta
                    const double Delta = sqrt( f.b*f.b - f.a*f.c );
                    




                    return 0;
                }
            };
        }
        

        outcome components:: brew1D(const double            K,
                                    const readable<double> &C0,
                                    writable<double>       &Cs) const
        {

            // initialize
            assert(Cs.size()>=C0.size());
            assert(are_valid(C0));
            iota::load(Cs,C0);

            std::cerr << "C=" << Cs << std::endl;

            if( reac.has_missing_actor(Cs) && prod.has_missing_actor(Cs) )
            {
                //--------------------------------------------------------------
                //
                // blocked!
                //
                //--------------------------------------------------------------
                std::cerr << "[BLOCKED]" << std::endl;
                return outcome(degenerated_extent,blocked_components,0);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // running!
                //
                //--------------------------------------------------------------
                ZMassAction F = { *this, K, Cs };

                // cycle
                const double    f0 = mass_action(K,Cs);
                const sign_type s0 = __sign::of(f0);

                if(__zero__ == s0)
                {
                    // early return
                    std::cerr << "Early Return" << std::endl;
                    const double xi = deduce(C0,Cs);
                    return outcome(qualify_extent(xi,Cs),running_components,xi);
                }
                else
                {
                    // updating according to limiting extent(s)
                    const limits    &lim = private_limits(Cs,0);
                    triplet<double>  x   = {0, 0,  0};
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

                    // find relative xi
                    std::cerr << "x=" << x << std::endl;
                    std::cerr << "f=" << f << std::endl;

                    const double xiNew = F.solve(x,f);
                    std::cerr << "xiNew=" << xiNew << std::endl;
                    exit(1);
                }




            }



            return outcome(significant_extent,running_components,0);
        }

    }


}
