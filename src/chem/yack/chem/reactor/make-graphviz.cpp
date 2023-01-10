#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {

        static inline void sp_viz(ios::ostream  &fp,
                                  const species &s,
                                  const char    *attr)
        {
            s.logo(fp) << '[';
            s.add_label(fp,s.name());
            fp << attr;
            s.end(fp << ']');
        }

        static inline void eq_viz(ios::ostream &fp,
                                  const equilibrium &eq,
                                  const char *attr)
        {

            eq.logo(fp) << '[';
            eq.add_label(fp,eq.name());
            fp << attr;
            eq.end(fp << ']');

            for(const actor *a = eq.reac->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                s.link(fp,&eq);
                if(nu>1) fp("[label=\"%u\"]",nu);
                s.end(fp);
            }

            for(const actor *a = eq.prod->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                eq.link(fp,&s);
                if(nu>1) fp("[label=\"%u\"]",nu);
                eq.end(fp);
            }
        }

        void cluster:: viz(ios::ostream &fp) const
        {
            fp << "subgraph cluster_";
            fp("%u",gvidx);
            fp << " {\n";

            // write all species
            for(const sp_gnode *sn=breed->conserved->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=rectangle,style=filled");
            }

            for(const sp_gnode *sn=breed->unbounded->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=rectangle");
            }


            // write all equilibria
            for(const eq_gnode *en=genus->bounded->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=oval,style=bold");
            }

            for(const eq_gnode *en=genus->roaming->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=octagon");
            }

            for(const eq_gnode *en=genus->special->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=octagon,style=dashed");
            }


            fp << "}\n";
        }

    }

}

