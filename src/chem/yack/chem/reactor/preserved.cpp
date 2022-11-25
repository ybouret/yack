
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
            YACK_XMLSUB(xml,"preserved");

            corelib(std::cerr << "Cin=","",C0);

            Qb.ld(true);
            Qg.ld(0);

        CYCLE:
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
                std::cerr << std::setw(15) << rg << " @" << rs << std::endl;
                if( rg>0 && (gain<0 || rg<gain) )
                {
                    gain = rg;
                    best = &rs;
                }

            }

            if(best)
            {
                YACK_XMLOG(xml, "-- gain = " << std::setw(15) << gain << " @" << *best);
                const size_t i = **best;
                iota::load(C0,Qm[i]);
                Qb[i] = false;
                Qg[i] = gain;
                goto CYCLE;
            }

            corelib(std::cerr << "Cout=","",C0);
            return xadd.tableau(Qg);
        }

    }

}

