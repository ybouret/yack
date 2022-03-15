
#include "yack/chem/plexus.hpp"

namespace yack
{

    namespace chemical
    {

        void plexus:: correctExtent(const readable<double> &C) throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.correct>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium      &eq = ***node;
                const size_t            ei = *eq;
                const limits           &lim = eq.primary_limits(C,eqs.width);
                double                 &xi1 = xi[ei];
                const double            xi0 = xi1;

                lim.clamp(xi1);

                if(entity::verbose)
                {
                    eqs.pad(std::cerr << "      @<" << eq.name << ">",eq) << " : " << lim << std::endl;
                    std::cerr << "      |_[ " << xi0 << " -> ";
                    std::cerr << xi1 << " ]" << std::endl;
                }
            }
            YACK_CHEM_PRINTLN("//   <plexus.correct/>");
        }

    }

}
