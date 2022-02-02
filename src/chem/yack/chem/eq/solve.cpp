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
                        const species   &sp = cm.sp;
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
                                  writable<double> &Ctry) const throw()
        {

            std::cerr << "<" << name << " solving@" << C << ">" << std::endl;
            assert(size()>0);
            assert(is_neutral());
            assert(K0>0);
            eqzcall g = { *this, K0, C, Ctry };


            triplet<double> x = { 0,      0, 0 };
            triplet<double> f = { g(x.a), 0, 0 };

            switch( __sign::of(f.a) )
            {
                case __zero__:
                    std::cerr << "\tGamma=0" << std::endl;
                    // reached or blocked
                    break;

                case positive: {
                    // need to move forward
                    std::cerr << "\tGamma>0" << std::endl;
                    const limiting * const lim = reac.find_limiting(C);
                    if(lim)
                    {
                        std::cerr << "\tlimited by " << (**(lim->pa)).name << std::endl;
                        f.c = g(x.c=lim->xi);
                        std::cerr << "x=" << x << ", f=" << f << std::endl;
                    }
                    else
                    {
                        std::cerr << "\tnot limited " << std::endl;
                    }
                } break;

                case negative:
                    // need to move reverse
                    std::cerr << "\tGamma<0" << std::endl;
                    const limiting * const lim = prod.find_limiting(C);
                    if(lim)
                    {
                        std::cerr << "\tlimited by " << (**(lim->pa)).name << std::endl;
                        f.c = g(x.c=-lim->xi);
                        std::cerr << "x=" << x << ", f=" << f << std::endl;
                    }
                    else
                    {
                        std::cerr << "\tnot limited " << std::endl;
                    }
                    break;
            }

            std::cerr << "<" << name << " solving/>" << std::endl;

        }
        

    }
}
