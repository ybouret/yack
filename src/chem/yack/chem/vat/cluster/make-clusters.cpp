#include "yack/chem/vat.hpp"

namespace yack
{
    namespace Chemical
    {

        void  tryReduceClusters(Clusters &source) noexcept
        {
            Clusters reduced;
            while(source.size>0)
            {
                Cluster *cl = source.pop_front();

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
            source.swap_with(reduced);
        }

        void Vat:: makeClusters(const xmlog      &xml,
                                Equilibria       &eqs,
                                Extended::Vector &usr)
        {
            YACK_XMLSUB(xml, "Vat::makeCluster");

            //------------------------------------------------------------------
            //
            // make all clusters by connectivity
            //
            //------------------------------------------------------------------
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
                    tryReduceClusters(*this);
                else
                    push_back( new Cluster(eq) );
            }

            //------------------------------------------------------------------
            //
            // finalize all clusters
            //
            //------------------------------------------------------------------
            for(Cluster *cl=head;cl;cl=cl->next)
                cl->finalize(xml,eqs,usr);

            //------------------------------------------------------------------
            //
            // update global equilibria
            //
            //------------------------------------------------------------------
            eqs.finalize();

            
        }

        void Vat:: updateK( Extended::Vector &K, const double t)
        {
            for(Cluster *cl=head;cl;cl=cl->next) cl->updateK(K,t);
        }

    }

}

