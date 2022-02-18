
#include "yack/chem/equilibrium.hpp"
#include "yack/math/root/zrid.hpp"
#include "yack/exception.hpp"
#include "yack/sort/sum.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double equilibrium:: extent(const double            K0,
                                    const readable<double> &C0,
                                    writable<double>       &Ctmp) const
        {
            switch(kind)
            {
                case family_0_1: {
                    assert(0==reac.size); assert(1==prod.size); assert(1==prod.head->nu);
                    return K0-(**prod.head)(C0);
                }

                case family_1_0: {
                    assert(0==prod.size); assert(1==reac.size); assert(1==reac.head->nu);
                    return (K0*(**reac.head)(C0)-1.0)/K0;
                }

                case family_0_2: {
                    assert(0==reac.size); assert(1==prod.size); assert(2==prod.head->nu);
                    return 0.5*(sqrt(K0)-(**prod.head)(C0));
                }

                case family_1_1: {
                    assert(1==reac.size); assert(1==reac.head->nu); assert(1==prod.size); assert(1==prod.head->nu);
                    return  (K0*(**reac.head)(C0)-(**prod.head)(C0))/(1+K0);
                }

                case family_0_11: {
                    assert(0==reac.size); assert(2==prod.size); assert(1==prod.head->nu); assert(1==prod.tail->nu);
                    const double A = (**prod.head)(C0);
                    const double B = (**prod.tail)(C0);
                    const double Delta = 4.0 * K0 + squared(A-B);
                    return 0.5*(sqrt(Delta)-(A+B));
                }

                case family_1_11: {
                    assert(1==reac.size); assert(1==reac.head->nu); assert(2==prod.size); assert(1==prod.head->nu); assert(1==prod.tail->nu);
                    const double A     = (**reac.head)(C0);
                    const double B     = (**prod.head)(C0);
                    const double C     = (**prod.tail)(C0);
                    const double Delta = squared(K0) + squared(B-C) + twice(K0*(sorted::sum(twice(A),B,C)));
                    return 0.5*(sqrt(Delta)-sorted::sum(K0,B,C));
                }


                case family_any:
                    break;
            }

            return extent_(K0,C0,Ctmp);
        }



        namespace {

            // wrapper to build a changed concentration
            struct eqz
            {
                // user's data
                const equilibrium       &eq;
                const double             K0;
                const readable<double>  &C;
                writable<double>        &Ctmp;

                // setting a checked trial and computing its mass action
                double operator()(const double xi)
                {
                    return eq.mass_action(K0,eq.move(Ctmp,C,xi));
                }

            };

        }





        double equilibrium:: extent_(const double            K0,
                                     const readable<double> &C,
                                     writable<double>       &Ctmp) const
        {

            triplet<double> x   = {0,0,0};
            triplet<double> g   = {0,mass_action(K0,C),0};
            if( fabs(g.b) <= 0)
            {
                return 0;
            }
            else
            {
                eqz             G   = { *this, K0, C, Ctmp };
                const limits   &lim = find_private_limits(C);
                //std::cerr << lim << std::endl;
                switch(lim.type)
                {
                    case limited_by_none:
                        throw exception("<%s> limited_by_none is impossible",name());

                    case limited_by_both:
                        // x in [-lim.prod->xi,lim.reac->xi]
                        g.c = mass_action(K0,lim.reac->pa->ldz( move(Ctmp,C,x.c=(lim.reac->xi)) ));
                        if(g.c>0) throw exception("<%s> limited_by_both invalid full forward mass action=%g",name(),g.c);

                        g.a = mass_action(K0,lim.prod->pa->ldz(move(Ctmp,C,x.a=(-lim.prod->xi))));
                        if(g.a<0) throw exception("<%s> limited_by_both invalid full reverse mass action=%g",name(),g.a);

                        assert(g.a*g.c<=0);
                        break;

                    case limited_by_reac:
                        // x in [-infty:lim.reac->xi]
                        assert(!nu_p);
                        assert(nu_r>0);
                        assert(d_nu<0);

                        g.c = mass_action(K0,lim.reac->pa->ldz( move(Ctmp,C,x.c=(lim.reac->xi)) ));
                        if(g.c>0) throw exception("<%s> limited_by_reac invalid full forward mass action=%g",name(),g.c);

                        x.a = -max_of( pow(K0,sexp), reac.maximum(C) );
                        g.a = G(x.a);
                        while(g.a<0)
                            g.a = G(x.a*=2);
                        break;

                    case limited_by_prod:
                        // x in [-lim.prod->xi:infty]
                        assert(!nu_r);
                        assert(nu_p>0);
                        assert(d_nu>0);

                        g.a = mass_action(K0,lim.prod->pa->ldz(move(Ctmp,C,x.a=(-lim.prod->xi))));
                        if(g.a<0) throw exception("<%s> limited_by_prod invalid full reverse mass action=%g",name(),g.a);

                        x.c = max_of( pow(K0,sexp), prod.maximum(C) );
                        g.c = G(x.c);
                        while(g.c>0)
                            g.c = G(x.c*=2);
                        break;
                }

                // solve
                zrid<double> zfind;
                if(!zfind(G,x,g)) throw exception("<%s> singular composition",name());
                return x.b;
            }
        }
        
    }

}

