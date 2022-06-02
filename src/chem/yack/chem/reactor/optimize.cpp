#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/minimize.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: meanGammaSquared(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtmp[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtmp,sorted::by_value)/N;
        }

        double reactor:: operator()(const double u) throw()
        {
            assert(u>=0);
            assert(u<=1);
            const double omu = 1.0 - u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j]        = omu * Corg[j] + u * Cend[j];
            }
            return meanGammaSquared(Ctry);
        }

        double reactor:: optimizeDecreaseFrom(const double G0) throw()
        {
            triplet<double> u = { 0, -1, 1 };
            triplet<double> g = { G0, -1, meanGammaSquared(Cend) };
            (void) minimize::find<double>::run_for(*this,u,g, minimize::inside);

            return g.b; //!< and Ctry is computed
        }

        double reactor:: selectDecreasedState() throw()
        {
            // initialize
            const enode       *eOpt = eqs.head();
            double             gOpt = Gtot[****eOpt];

            // look up on single
            for(const enode *node=eOpt->next;node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            // look up on couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            // update
            const equilibrium &opt = ***eOpt;
            transfer(Corg,Ctot[*opt]);
            if(verbose)
            {
                couples.pad(std::cerr << vpfx << opt.name,opt) << " : selected " << gOpt << std::endl;
            }
            return gOpt;
        }

    }
}

