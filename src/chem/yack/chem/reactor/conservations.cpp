
#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {


        void cluster:: conservations(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"cluster::conservations");
            const size_t n = genus->bounded->size;

            if(n<=0) {
                YACK_XMLOG(xml, "no bounded equilibrium");
                return;
            }

            if(breed->conserved->size<=0) {
                YACK_XMLOG(xml, "no conserved species");
                return;
            }

            const size_t m = (*alive)->size;
            matrix<int>  nu(n,m);
            {
                size_t i=1;
                for(const eq_gnode *en=genus->bounded->head;en;en=en->next)
                {
                    
                }
            }

        }

    }

}

