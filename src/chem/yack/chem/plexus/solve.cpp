#include "yack/chem/plexus.hpp"
#include "yack/chem/eqs/cross.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex/integer.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double plexus:: hamiltonian(const readable<double> &C)
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = eq.mass_action(K[ei],C);
            }
            return sorted::sum(Xtry,sorted::by_value)/N;
        }

        double plexus:: operator()(const double u)
        {
            const double v = 1.0-u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = v * Corg[j] + u * Cend[j];
            }
            return hamiltonian(Ctry);
        }


        double plexus:: computeLatticeExtent()
        {
            // summing |Xi| on singles
            double sumAbsXi = 0;

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       ax = fabs( Xl[ei] = eq.solve1D(Kl[ei],Corg,Cs[ei]) );
                sumAbsXi += ax;
            }

            if(sumAbsXi<=0)
            {
                // cleaning up
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    Xl[ei] = 0;
                }

            }
            else
            {
                // computing couples solution
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    Xl[ei] = eq.solve1D(Kl[ei],Corg,Cs[ei]);
                }

            }

            if(verbose) lattice(std::cerr << vpfx << "Xi=", Xl, vpfx);

            return sumAbsXi;

        }

        bool plexus:: solve( writable<double> &C0 ) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "PlexusSolve");
            assert(C0.size()>=M);

            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            // initializing
            switch(N)
            {
                case 0:   // no equilibrium
                    return true;

                case 1: { // special case
                    const equilibrium &eq = *** singles.head();
                    (void) eq.solve1D(K[*eq],C0,Corg);
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS_1D/>");
                } return true;

                default: // generic case, prepare workspace
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = C0[j];
                    }
            }

            computeLatticeExtent();

            


            return false;

        }

    }

}
