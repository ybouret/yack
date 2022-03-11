#include "yack/chem/plexus.hpp"

namespace yack
{

    namespace chemical
    {

        void plexus:: computeExcess(const readable<double> &C) throw()
        {
            const size_t M_ = M;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const double       Ki = K[ei];
                 for(size_t j=M_;j>0;--j) Ci[j] = C[j];
                Xi[ei] = eq.solve1D(Ki,C,Ci);
                eq.drvs_action(Psi[ei],Ki,Ci,Ctmp);
            }
        }

    }

}

