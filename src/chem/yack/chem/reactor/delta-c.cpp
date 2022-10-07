#include "yack/chem/reactor.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/arith/round10.hpp"

#include <iomanip>

namespace yack
{

    namespace chemical
    {
        bool reactor:: forwardingC(const xmlog &xml)
        {
            
            YACK_XMLSUB(xml, "forwarding");

            //----------------------------------------------------------
            //
            // compute and monitor working dC
            //
            //----------------------------------------------------------
            bool           usingFullLength  = true;
            double         reductionFactor  = 1.0;
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;


                xadd.ldz();
                for(size_t i=N;i>0;--i) xadd.ld( NuA[i][j] * xi[i] );
                const double d = (dC[j] = xadd.get());
                const double c = Corg[j];
                if(verbose)
                {
                    corelib.pad(*xml << '[' << sp.name <<']',sp) << " = " << std::setw(15) << c;
                    if(d>=0)
                    {
                        std::cerr << " +" << std::setw(15) << d;
                    }
                    else
                    {
                        std::cerr << " -" << std::setw(15) << fabs(d);
                    }
                }

                if(d<0 && (-d)>c)
                {
                    //----------------------------------------------------------
                    // should be only for secondary species
                    // but could numerically happen for primary
                    //----------------------------------------------------------
                    const double scaling = c/(-d);
                    if(verbose) std::cerr << " => scaling@" << scaling;
                    usingFullLength = false;
                    reductionFactor = min_of(reductionFactor,scaling);
                }

                if(verbose) std::cerr << std::endl;
            }

            if(!usingFullLength)
            {
                YACK_XMLOG(xml,"-- reductionFactor = " << std::setw(15) << reductionFactor);
                reductionFactor = math:: round10<double>:: floor_with<1>(reductionFactor);
                YACK_XMLOG(xml,"-- reductionFactor = " << std::setw(15) << reductionFactor << " (updated)");
            }
            

            //----------------------------------------------------------
            //
            // compute Cend
            //
            //----------------------------------------------------------
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                Cend[j] = max_of<double>(Corg[j]+reductionFactor*dC[j],0);
            }


            return usingFullLength;
        }
    }

}
