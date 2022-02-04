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
            struct eqzcall
            {
                const equilibrium       &eq;
                const double             K0;
                const readable<double>  &C;
                writable<double>        &Ctry;

                double operator()(const double xi)
                {
                    for(const cnode *node=eq.head();node;node=node->next)
                    {
                        const component &cm = ***node;
                        const species   &sp = *cm;
                        const size_t     j  = sp.indx; assert(j>=1); assert(j<=C.size()); assert(j<=Ctry.size());
                        const unit_t     nu = cm.nu;
                        const double     Cj = C[j];    assert(C[j]>=0);
                        Ctry[j] = max_of<double>(0,Cj + nu * xi);
                    }
                    return eq.mass_action(K0,Ctry);
                }
            };

        }

        void equilibrium::  solve(const double      K0,
                                  writable<double> &C,
                                  writable<double> &Ctry) const
        {

            std::cerr << "<" << name << " solving@" << C << ">" << std::endl;
            assert(size()>0);
            assert(is_neutral());
            assert(K0>0);

            eqzcall         g = { *this, K0, C, Ctry };
            triplet<double> x = { 0,      0, 0 };
            triplet<double> f = { g(x.a), 0, 0 };

            switch( __sign::of(f.a) )
            {
                case __zero__:
                    //----------------------------------------------------------
                    // reached or blocked
                    //----------------------------------------------------------
                    std::cerr << " Gamma=0" << std::endl;
                    break;

                case positive: {
                    //----------------------------------------------------------
                    // need to move forward
                    //----------------------------------------------------------
                    std::cerr << " Gamma>0" << std::endl;
                    const limiting * const lim = reac.find_limiting(C);
                    if(lim)
                    {
                        std::cerr << " limited by " << (**(lim->pa)).name << std::endl;
                        f.c = g(x.c=lim->xi);
                    }
                    else
                    {
                        std::cerr << " not limited " << std::endl;
                        assert(d_nu>0);
                        f.c = g(x.c=pow(K0,sexp));
                        while(f.c>0)
                        {
                            f.c = g(x.c *= 2);
                        }
                    }

                } break;

                case negative:
                    //----------------------------------------------------------
                    // need to move reverse
                    //----------------------------------------------------------
                    std::cerr << " Gamma<0" << std::endl;
                    const limiting * const lim = prod.find_limiting(C);
                    if(lim)
                    {
                        std::cerr << " limited by " << (**(lim->pa)).name << std::endl;
                        f.c = g(x.c=-lim->xi);
                    }
                    else
                    {
                        std::cerr << " not limited " << std::endl;
                        assert(d_nu<0);
                        //std::cerr << "Cs=" << pow(K0,sexp) << std::endl;
                        f.c = g(x.c=-pow(K0,sexp));
                        while(f.c<0)
                        {
                            f.c = g(x.c *= 2);
                        }
                    }
                    break;
            }
            std::cerr << " x=" << x << ", f=" << f << std::endl;
            zrid<double>    Z;
            if(!Z(g,x,f))
            {
                
            }

            std::cerr << " x=" << x << ", f=" << f << std::endl;
            set(C,Ctry);
            std::cerr << "<" << name << " solving/>" << std::endl;
        }
        

    }
}
