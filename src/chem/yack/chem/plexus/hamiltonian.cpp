#include "yack/chem/plexus.hpp"
#include "yack/sort/sum.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {
        //----------------------------------------------------------------------
        //
        // hamiltonian of the system
        //
        //----------------------------------------------------------------------
        double plexus:: hamiltonian(const readable<double> &C)
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtry,sorted::by_value)/N;
        }

        //----------------------------------------------------------------------
        //
        // hamiltonian( (1-u) * Corg + u * Cend )
        //
        //----------------------------------------------------------------------
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
    }

}


