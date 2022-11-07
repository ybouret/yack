#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace chemical
    {
        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);

            if(N<=0)
            {
                YACK_XMLOG(xml,"-- no equilibrium");
                return true;
            }

            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Cend[j] = Ctry[j] = C0[j];
                dC[j]   = 0;
            }

            // primary balance
            {
                YACK_XMLSUB(xml,"PrimaryBalancing");
                bool primaryBalanced = true;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    if(eq.try_primary_balance(Corg))
                    {
                        YACK_XMLOG(xml, "[+] " << eq.name);
                    }
                    else
                    {
                        YACK_XMLOG(xml, "[-] " << eq.name);
                        primaryBalanced = false;
                    }

                }
                if(!primaryBalanced)
                {
                    YACK_XMLOG(xml, "-- failure!!");
                    return false;
                }
            }



            return true;
        }

    }

}

