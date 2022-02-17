
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
                                    writable<double>       &Ctmp,
                                    const species *        &vanishing) const
        {
            assert(NULL==vanishing);
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

            return extent_(K0,C0,Ctmp,vanishing);
        }

        double equilibrium:: extent_(const double            K0,
                                     const readable<double> &C,
                                     writable<double>       &Ctmp,
                                     const species *        &vanishing) const
        {

            assert(NULL==vanishing);

            const double g0  = mass_action(K0,C);
            switch( __sign::of(g0) )
            {

                case __zero__: return 0;
                case positive: return xfwd(K0,C,Ctmp,g0,vanishing);
                case negative: return xrev(K0,C,Ctmp,g0,vanishing);
            }
            throw exception("Never Get Here");
        }


        void equilibrium:: move(writable<double> &Ctmp, const readable<double> &C, const double xi) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const component &cm = ***node;
                const species   &sp = *cm;
                const size_t     j  = *sp; assert(j>=1); assert(j<=C.size()); assert(j<=Ctmp.size());
                const unit_t     nu = cm.nu;
                const double     Cj = C[j];    assert(C[j]>=0);
                Ctmp[j] = max_of<double>(0,Cj + nu * xi);
            }
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
                    eq.move(Ctmp,C,xi);
                    return eq.mass_action(K0,Ctmp);
                }

                inline void solve(triplet<double> &x,
                                  triplet<double> &f,
                                  const string    &name)
                {
                    zrid<double> zfind;
                    if(!zfind(*this,x,f))
                        throw exception("%s: cannot solve <%s>", equilibrium::clid, name() );
                }
            };

        }


        double equilibrium:: xfwd(const double             K0,
                                  const  readable<double> &C,
                                  writable<double>        &Ctmp,
                                  const double             g0,
                                  const species *         &vanishing) const
        {
            // move forward, limit is reactant
            assert(g0>0);
            const limiting *lim = reac.find_primary_limiting(C);
            triplet<double> x   = { 0,  0, 0 };
            triplet<double> f   = { g0, 0, 0 };
            eqz             g   = { *this, K0, C, Ctmp };
            if(lim)
            {
                f.c = g(x.c=lim->xi);
                if(f.c>0)
                {
                    // full forward...
                    x.b = x.c;
                    f.b = f.c;
                    vanishing = & **(lim->pa);
                    goto DONE;
                }
                assert(f.c<=0);
            }
            else
            {
                assert(d_nu>0);
                x.c = max_of( reac.maximum(C), pow(K0,sexp) );
                f.c = g(x.c);
                while(f.c>0)
                {
                    f.c = g(x.c *= 2);
                }
            }
            g.solve(x,f,name);

        DONE:
            return x.b;
        }


        double equilibrium:: xrev(const double             K0,
                                  const  readable<double> &C,
                                  writable<double>        &Ctmp,
                                  const double             g0,
                                  const species *         &vanishing) const
        {
            // move reverse, limit is product
            assert(g0<0);
            const limiting *lim = prod.find_primary_limiting(C);
            triplet<double> x   = { 0,  0, 0 };
            triplet<double> f   = { g0, 0, 0 };
            eqz             g   = { *this, K0, C, Ctmp };
            if(lim)
            {
                f.c = g(x.c=-lim->xi);
                if(f.c<0)
                {
                    // full reverse
                    x.b = x.c;
                    f.b = f.c;
                    vanishing = & **(lim->pa);
                    goto DONE;
                }
            }
            else
            {
                assert(d_nu<0);
                x.c = - max_of( prod.maximum(C), pow(K0,sexp) );
                f.c = g(x.c);
                while(f.c<0)
                {
                    f.c = g(x.c *= 2);
                }
            }
            g.solve(x,f,name);

        DONE:
            return x.b;
        }

    }

}

