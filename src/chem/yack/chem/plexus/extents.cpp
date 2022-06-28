
#include "yack/chem/plexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {
        double plexus:: computePartialExtent(const double       G0,
                                             const equilibrium &eq) throw()
        {
            //------------------------------------------------------------------
            //
            // compute extent and solution in Cend
            //
            //------------------------------------------------------------------
            const size_t      ei  = *eq;
            const double      ax  = fabs( Xl[ei] = eq.solve1D(Kl[ei],Corg,Cend) );
            writable<double> &Ci  = Cs[ei];
            bool             &ok  = Ok[ei];

            if(ax>0)
            {
                //--------------------------------------------------------------
                //
                // out of equilibrium
                //
                //--------------------------------------------------------------
                const double G1 = hamiltonian(Cend);
                if(G1<G0)
                {
                    //----------------------------------------------------------
                    // keep new extent @Cend
                    //----------------------------------------------------------
                    ok = true; iota::load(Ci,Cend);
                    if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : [winning] " << G1 << std::endl;
                }
                else
                {
                    //----------------------------------------------------------
                    // compensated by other equilibria => stalled @Corg
                    //----------------------------------------------------------
                    ok = false; iota::load(Ci,Corg);
                    if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : [stalled] " << G0 << std::endl;
                }
            }
            else
            {
                assert(ax<=0);
                //--------------------------------------------------------------
                //
                // numerical equilibrium, |Xi|<=0, keep Corg
                //
                //--------------------------------------------------------------
                ok = true; iota::load(Ci,Corg);
                if(verbose) lattice.pad(std::cerr << vpfx << eq.name,eq) << " : [already] " << G0 << std::endl;
            }

            return ax;
        }

        double plexus:: computeSinglesExtent(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeSinglesExtent");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = computePartialExtent(G0,***node); assert(Xtry[ei]>=0);
            }
            return sorted::sum(Xtry,sorted::by_value);
        }

        void plexus:: computeCouplesExtent(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "computeCouplesExtent");
            for(const enode *node=couples.head();node;node=node->next)
            {
                (void) computePartialExtent(G0,***node);
            }
        }

    }

}

