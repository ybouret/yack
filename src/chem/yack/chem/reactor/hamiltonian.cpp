
#include "yack/chem/reactor.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/iota.hpp"

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

        double reactor:: Hamiltonian(const group &g)
        {
            //------------------------------------------------------------------
            // build a trial concentration
            //------------------------------------------------------------------
            math::iota::load(Ctry,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium &member = **ep;
                member.transfer(Ctry,Ceq[*member]);
            }

            //------------------------------------------------------------------
            // compute it's hamiltonian
            //------------------------------------------------------------------
            return Hamiltonian(Ctry);
        }


    }
}


