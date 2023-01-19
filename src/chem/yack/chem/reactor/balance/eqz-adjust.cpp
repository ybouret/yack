#include "yack/chem/reactor/balance/equalizer.hpp"

namespace yack {


    namespace chemical
    {
        void equalizer:: adjust_reac(writable<double> &C, const eq_group &reac_only, const xmlog &xml)
        {
            for(const eq_gnode *gn=reac_only.head;gn;gn=gn->next)
            {
                const equilibrium &eq = (***gn).host; assert(eq.reac->size>0); assert(eq.prod->size<=0);
                if(wall.adjust(C,eq.reac))
                {
                    const double     xi = wall.xi;
                    for(const cnode *cn = eq.head();cn;cn=cn->next)
                    {
                        const component &a = ***cn; assert(a.nu<0);
                        const species   &s = *a;
                        const size_t     j = *s;
                        C[j] = max_of<double>( C[j] + (-a.nu) * xi, 0.0);
                    }
                    wall.vanish(C);
                    if(xml.verbose)
                    {
                        *xml << "-- with <" << eq.name << "> " << wall << " : ";
                        eq.display_compact(std::cerr,C) << std::endl;
                    }
                }
            }

        }

        void equalizer:: adjust_prod(writable<double> &C, const eq_group &prod_only, const xmlog &xml)
        {
            for(const eq_gnode *gn=prod_only.head;gn;gn=gn->next)
            {
                const equilibrium &eq = (***gn).host; assert(eq.prod->size>0); assert(eq.reac->size<=0);
                if(wall.adjust(C,eq.prod))
                {
                    const double     xi = wall.xi;
                    for(const cnode *cn = eq.head();cn;cn=cn->next)
                    {
                        const component &a = ***cn; assert(a.nu>0);
                        const species   &s = *a;
                        const size_t     j = *s;
                        C[j] = max_of<double>( C[j] + a.nu * xi, 0.0);
                    }
                    wall.vanish(C);
                    if(xml.verbose)
                    {
                        *xml << "-- with <" << eq.name << "> " << wall << " : ";
                        eq.display_compact(std::cerr,C) << std::endl;
                    }
                }
            }

        }




        void equalizer:: adjust(writable<double> &C,
                                const cluster    &cc,
                                const xmlog      &xml)
        {
            YACK_XMLSUB(xml, "equalize::adjust::cluster" );
            adjust_reac(C,*(cc.replica->genus->reac_only),xml);
            adjust_prod(C,*(cc.replica->genus->prod_only),xml);
        }

        void equalizer:: adjust(writable<double> &C, const xmlog &xml)
        {
            YACK_XMLSUB(xml, "equalize::adjust" );
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
                adjust(C,*cc,xml);
        }

    }

}

