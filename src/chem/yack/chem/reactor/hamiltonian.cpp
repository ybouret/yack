
#include "yack/chem/reactor.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    
    namespace chemical
    {

        double reactor:: Hamiltonian(const readable<double> &C)
        {
            xadd.ldz();

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(blocked[ei]) continue;
                assert(sigma[ei]<0);
                xadd.ld( squared(eq.mass_action(K[ei],C,xmul)/sigma[ei]) );
            }

            return sqrt(xadd.get());
        }

    }
}


