#include "yack/chem/vat.hpp"

namespace yack
{
    namespace Chemical
    {

        void Vat:: makeClusters(const xmlog &xml,
                                Equilibria  &eqs)
        {
            YACK_XMLSUB(xml, "Vat::makeCluster");
            for(eNode *en=eqs->head;en;en=en->next)
            {
                bool         ok = false;
                Equilibrium &eq = ***en;
                for(Cluster *cl=head;cl;cl=cl->next)
                {
                    if(cl->recruits(eq))
                    {
                        ok = true;
                        (*cl) << eq;
                        break;
                    }
                }

                if(ok)
                    tryReduce();
                else
                    push_back( new Cluster(eq) );
            }
            for(Cluster *cl=head;cl;cl=cl->next)
            {
                cl->finalize(xml);
            }
            
        }

        void Vat:: tryReduce() noexcept
        {
            Clusters reduced;
            while(size>0)
            {
                Cluster *cl = pop_front();

                for(Cluster *red=reduced.head;red;red=red->next)
                {
                    if(red->linkedTo(*cl))
                    {
                        red->merge_back(*cl);
                        delete cl;
                        cl = 0;
                    }
                }

                if(cl)
                    reduced.push_back(cl);
            }
            swap_with(reduced);
        }
    }

}

