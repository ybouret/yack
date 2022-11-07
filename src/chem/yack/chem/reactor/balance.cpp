#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"

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


            //------------------------------------------------------------------
            //
            // primary balance directly in C0
            //
            //------------------------------------------------------------------
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
                    YACK_XMLOG(xml, "-- [[ FAILURE ]]");
                    return false;
                }
            }

            if(verbose) corelib(*xml << "-- Cbal=","", C0);

            //------------------------------------------------------------------
            //
            // check/init replica search
            //
            //------------------------------------------------------------------

            {
                
                YACK_XMLSUB(xml,"ReplicaBalancing");
                Bal.free();
                for(const snode *node = corelib.head();node;node=node->next)
                {
                    const species &s = ***node;
                    const size_t   j = *s;
                    Corg[j] = Cend[j] = Ctry[j] = C0[j];
                    dC[j]   = 0;
                    switch(s.rank)
                    {
                        case 0:
                            break;

                        case 1:
                            assert(Corg[j]>=0);
                            break;

                        default: {
                            const double Cj = Corg[j];
                            if(Cj<0)
                            {
                                Bal << squared( dC[j] = -Cj );
                            }

                        } break;
                    }
                }
            }

            const double B0 = xadd.tableau(Bal);
            std::cerr << "Bal=" << Bal << std::endl;
            std::cerr << "B0 = " << B0 << std::endl;
            std::cerr << "dBdC=" << dC << std::endl;


            if(B0)
            {
                std::cerr << singles << std::endl;
                NuA.assign(Nu);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const components::state es = eq.state_at(Corg);
                    if(es == components::are_blocked)
                    {
                        blocked[ei] = true;
                        NuA[ei].ld(0);
                        xi[ei]      = 0;
                    }
                    else
                    {
                        blocked[ei] = false;
                        xi[ei]      = xadd.dot(NuA[ei],dC);
                    }
                    const xlimits &lm = eq.primary_limits(Corg,corelib.maxlen);
                    singles.pad(std::cerr << eq.name,eq) << " : " << components::state_text(es) << ' ';
                    std::cerr << lm << std::endl;
                }
                singles(std::cerr << "xi=" , "", xi);




                exit(0);
            }



            return true;
        }

    }

}

