
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include <iomanip>
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool reactor:: optimizeFullStep(double &G0) throw()
        {

            //------------------------------------------------------------------
            //
            // Cstp = NuA' * xi and deduce limiting ratio
            //
            //------------------------------------------------------------------
            ratio.free();
            {
                const size_t n = N;
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    for(size_t i=n;i>0;--i)
                    {
                        Xtry[i] = xi[i] * NuA[i][j];
                    }
                    const double d = (Cstp[j] = sorted::sum(Xtry,sorted::by_abs_value));
                    if(d<0)
                    {
                        const double c = Corg[j]; assert(c>=0);
                        ratio << (c/-d);
                    }
                }
            }

            double expand = 2.0;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                std::cerr << vpfx << "ratio=" << ratio << std::endl;
                expand = min_of(expand,0.99*ratio.front());
            }

            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Cend[j] = max_of(Corg[j]+Cstp[j]*expand,0.0);
            }



            triplet<double> u = { 0,  -1, 1 };
            triplet<double> g = { G0, -1, Hamiltonian(Cend) };

            if(false)
            {
                ios::ocstream fp("hamiltonian.dat");
                const size_t  np = 1000;
                for(size_t i=0;i<=np;++i)
                {
                    const double uu = i/double(np);
                    fp("%.15g %.15g\n",uu, (*this)(uu));
                }
            }

            optimize::run_for(*this, u, g, optimize::inside);
            active.transfer(Corg,Ctry);

            if(verbose)
            {
                lib(std::cerr << vpfx << "Cfin=",Corg,vpfx);
                std::cerr << vpfx << "G: " << G0 << " --> " << g.b << " @" << u.b << std::endl;
            }

            if(g.b>=G0)
            {
                G0=g.b;
                return false;
            }
            else
            {
                G0=g.b;
                return true;
            }
        }

    }

}

