
#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/chem/outcome.hpp"

namespace yack
{

    namespace chemical
    {

        bool reactor:: returnSolved(writable<double> &C0, const xmlog &xml)
        {
            working.transfer(C0,Corg);
            YACK_XMLSUB(xml, "returnSolved");
            if(verbose)
            {
                corelib(*xml << "Cend=", "", C0);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const double       Ki  = K[ei];
                    const outcome      oc  = outcome::study(eq, Ki, Corg, Ctry, xmul, xadd);
                    singles.pad(*xml << eq.name,eq) << ' ' << oc << std::endl;
                }
            }
            return true;
        }

        
    }

}

