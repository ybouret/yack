
#include "yack/chem/reactor.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace chemical
    {

        double reactor:: operator()(const double u1)
        {
            const double u0 = 1.0 - u1;
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double C0 = Corg[j];
                const double C1 = Cend[j];
                double       Cmin=C0,Cmax=C1;
                if(Cmax<Cmin) cswap(Cmin,Cmax);
                Ctry[j] = clamp(Cmin,C0*u0 + C1*u1,Cmax);
            }
            return Hamiltonian(Ctry);
        }

    }

}

