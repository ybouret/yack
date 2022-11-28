
#include "yack/chem/reactor.hpp"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {


        double reactor:: preserved(writable<double> &C0, const xmlog &xml)
        {
            static const char fn[] = "preserving";
            YACK_XMLSUB(xml,fn);

            size_t ng = 0;
            Qb.ld(true);
            Qg.ld(0);

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- " << fn << " cycle #" << cycle << " --------");
            double             gain = -1;
            const restriction *best = NULL;
            for(size_t i=Nc;i>0;--i)
            {
                if(!Qb[i])
                {
                    continue;
                }
                const restriction &rs = *Qv[i]; assert(*rs==i);
                const double       rg =  rs.apply(Qm[*rs],C0,xadd);
                YACK_XMLOG(xml, "--  gain = " << std::setw(15) << rg << " @" << rs);
                if( rg>0 && (gain<0 || rg<gain) )
                {
                    gain = rg;
                    best = &rs;
                }

            }

            if(best)
            {
                ++ng;
                YACK_XMLOG(xml, "--> gain = " << std::setw(15) << gain << " @" << *best);
                const size_t i = **best;
                iota::load(C0,Qm[i]);
                Qb[i] = false;
                Qg[i] = gain;
                goto CYCLE;
            }

            const double injected = xadd.tableau(Qg);
            if(ng&&verbose)
            {
                corelib(*xml << "Cout=","",C0);
            }
            YACK_XMLOG(xml, "--> injected = " << injected);
            return injected;

        }
        
    }

}

