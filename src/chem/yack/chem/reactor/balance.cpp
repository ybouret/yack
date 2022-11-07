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



            // primary balance directly in C0
            {
                YACK_XMLSUB(xml,"PrimaryBalancing");
                bool primaryBalanced = true;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    YACK_XMLOG(xml, "--> " << eq.name);
                    if(eq.try_primary_balance(C0,xml))
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

            if(verbose) corelib(*xml << "-- Cbal=","", C0);



            return true;
        }

    }

}

