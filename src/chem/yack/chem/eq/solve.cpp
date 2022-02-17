#include "yack/chem/equilibrium.hpp"
#include "yack/type/utils.hpp"
#include "yack/exception.hpp"

#include "yack/math/root/zrid.hpp"

namespace yack
{
    namespace chemical
    {
        using namespace math;

        namespace {

            // wrapper to build a changed concentration
            struct eqz
            {
                // user's data
                const equilibrium       &eq;
                const double             K0;
                const readable<double>  &C;
                writable<double>        &Ctry;

                // setting a checked trial and computing its mass action
                double operator()(const double xi)
                {
                    for(const cnode *node=eq.head();node;node=node->next)
                    {
                        const component &cm = ***node;
                        const species   &sp = *cm;
                        const size_t     j  = *sp; assert(j>=1); assert(j<=C.size()); assert(j<=Ctry.size());
                        const unit_t     nu = cm.nu;
                        const double     Cj = C[j];    assert(C[j]>=0);
                        Ctry[j] = max_of<double>(0,Cj + nu * xi);
                    }
                    return eq.mass_action(K0,Ctry);
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

        void equilibrium::  solve(const double      K0,
                                  writable<double> &C,
                                  writable<double> &Ctry) const
        {
            const species *vanishing = NULL;
            const double   xi        = extent(K0,C,Ctry,vanishing);
            move(C,Ctry,xi);
            if(vanishing) (*vanishing)(C) = 0;

#if 0
            assert(size()>0);
            assert(is_neutral());
            assert(K0>0);
            const double    g0 = mass_action(K0,C);
            
            switch( __sign::of(g0) )
            {
                case __zero__:
                    //----------------------------------------------------------
                    // reached or blocked
                    //----------------------------------------------------------
                    break;

                case positive: {
                    //----------------------------------------------------------
                    // need to move forward
                    //---------------------------------------------------------_
                    zfwd(K0,C,Ctry,g0);
                    break;

                case negative:
                    //----------------------------------------------------------
                    // need to move reverse
                    //----------------------------------------------------------
                    zrev(K0,C,Ctry,g0);
                    break;
                }

            }
#endif
        }

        void equilibrium:: zfwd(const double      K0,
                                writable<double> &C,
                                writable<double> &Ctry,
                                const double      g0) const
        {
            assert(g0>0);
            assert(K0>0);
            eqz                    g = { *this, K0, C, Ctry };
            const limiting * const l = reac.find_private_limiting(C);
            triplet<double>        x = { 0,  0, 0 };
            triplet<double>        f = { g0, 0, 0 };
            if(l)
            {
                f.c = g(x.c=l->xi);
                if(f.c>0)
                {
                    // full forward...
                    x.b = x.c;
                    f.b = f.c;
                    (**(l->pa))(C) = 0;
                    goto DONE;
                }
            }
            else
            {
                assert(d_nu>0);
                f.c = g(x.c=pow(K0,sexp));
                while(f.c>0)
                {
                    f.c = g(x.c *= 2);
                }
            }
            g.solve(x,f,name);
            
        DONE:
            set(C,Ctry);
        }


        void equilibrium:: zrev(const double      K0,
                                writable<double> &C,
                                writable<double> &Ctry,
                                const double      g0) const
        {

            assert(g0<0);
            assert(K0>0);
            eqz                    g = { *this, K0, C, Ctry };
            const limiting * const l = prod.find_private_limiting(C);
            triplet<double>        x = { 0,  0, 0 };
            triplet<double>        f = { g0, 0, 0 };
            if(l)
            {
                f.c = g(x.c=-l->xi);
                if(f.c<0)
                {
                    // full reverse
                    x.b = x.c;
                    f.b = f.c;
                    (**(l->pa))(C) = 0;
                    goto DONE;
                }
            }
            else
            {
                assert(d_nu<0);
                f.c = g(x.c=-pow(K0,sexp));
                while(f.c<0)
                {
                    f.c = g(x.c *= 2);
                }
            }
            g.solve(x,f,name);

        DONE:
            set(C,Ctry);
        }

    }
}
