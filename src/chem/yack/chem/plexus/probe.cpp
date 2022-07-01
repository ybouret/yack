
#include "yack/chem/plexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/optimize.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        double plexus:: probeCombinedExtents(const double G0) throw()
        {
            //------------------------------------------------------------------
            //
            // build Cstp = Nu'*xi
            //
            //------------------------------------------------------------------
            ratio.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node; assert(Corg[j]>=0);
                for(size_t i=N;i>0;--i)
                {
                    Xtry[i] = NuA[i][j] * xi[i];
                }
                const double d = (Cstp[j] = sorted::sum(Xtry,sorted::by_abs_value));
                if(d<0)
                {
                    const double c = Corg[j];
                    ratio << (c/-d);
                }
            }


            //------------------------------------------------------------------
            //
            // check expansion
            //
            //------------------------------------------------------------------
            double expand = 2.0;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                YACK_CHEM_PRINTLN(vpfx << "ratio=" << ratio);
                expand = min_of(0.99 * ratio.front(),expand);
            }
            else
            {
                YACK_CHEM_PRINTLN(" [unlimited]");
            }

            //------------------------------------------------------------------
            //
            // build maxium search concentration
            //
            //------------------------------------------------------------------
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node; assert(Corg[j]>=0);
                Cend[j] = max_of(0.0,Corg[j]+expand*Cstp[j]);
            }

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

            triplet<double> u  = { 0, -1, 1 };
            triplet<double> g  = { G0, -1, hamiltonian(Cend) };
            optimize::run_for(*this,u,g,optimize::inside);

            iota::load(Corg,Ctry);
            return g.b;
        }

    }

}


