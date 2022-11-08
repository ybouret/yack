#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        double reactor:: Balance(const readable<double> &C)
        {
            Bal.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double c = C[j];
                if(c<0) Bal << squared(c);
            }
            return xadd.tableau(Bal)/2;
        }


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
            // initial primary balancing directly in C0
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

            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Cend[j] = Ctry[j]  = C0[j];
                dC[j]   = 0;
            }

            // compute gradient/balance
            Bal.free();
            bool bad = false;
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Corg[j];
                switch(s.rank)
                {
                    case 0:
                        break;

                    case 1:
                        assert(c>=0);
                        break;

                    default:
                        if(c<0)
                        {
                            Bal << squared(c);
                            dC[j]  = c;
                            bad    = true;
                        }
                        else
                        {
                            dC[j] = 0;
                        }
                }
            }
            double B0 =  xadd.tableau(Bal)/2;
            std::cerr << "Bal = " << Bal << std::endl;
            std::cerr << "B0  = " << B0   << std::endl;
            std::cerr << "S   = " << dC   << std::endl;

            if(bad)
            {
                size_t nrun = 0;
                NuA.assign(Nu);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium      &eq = ***node;
                    const size_t            ei = *eq;
                    const components::state es =  eq.state_at(Corg);
                    double                 &xx = xi[ei];
                    if(verbose) singles.pad(std::cerr << eq.name,eq) << " : " << eq.state_text(es);

                    switch (es) {
                        case components::are_blocked: {
                            // block whatever
                            NuA[ei].ld(0);
                            blocked[ei] = true;
                            xx          = 0;
                        } break;

                        case components::are_running: {
                            // guess
                            blocked[ei] = false;
                            xx          = xadd.dot(NuA[ei],dC);
                            std::cerr << '@' << std::setw(15) << xx;
                            // check limits
                            const xlimits &lm = eq.primary_limits(Corg,corelib.maxlen);
                            if(verbose) std::cerr << ' ' << lm;
                        } break;
                    }

                    if(verbose)  std::cerr << std::endl;

                }
                exit(0);
            }


            return true;
        }

    }

}

