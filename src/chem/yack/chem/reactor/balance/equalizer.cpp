

#include "yack/chem/reactor/balance/equalizer.hpp"

namespace yack {

    namespace chemical
    {

        equalizer:: ~equalizer() throw() {}


        equalizer:: equalizer(const reactor &_) :
        sp_fund( new sp_bank() ),
        cs(_),
        sf( *this )
        {
        }

        void equalizer:: adjust_reac(writable<double> &C, const eq_group &reac_only)
        {
            for(const eq_gnode *gn=reac_only.head;gn;gn=gn->next)
            {
                const equilibrium &eq = (***gn).host; assert(eq.reac->size>0); assert(eq.prod->size<=0);
                if(sf.adjust(C,eq.reac))
                {
                    const double     xi = sf.xi;
                    for(const cnode *cn = eq.head();cn;cn=cn->next)
                    {
                        const component &a = ***cn; assert(a.nu<0);
                        const species   &s = *a;
                        const size_t     j = *s;
                        C[j] = max_of<double>( C[j] + (-a.nu) * xi, 0.0);
                    }
                    sf.vanish(C);
                }
            }

        }

        void equalizer:: adjust_prod(writable<double> &C, const eq_group &prod_only)
        {
            for(const eq_gnode *gn=prod_only.head;gn;gn=gn->next)
            {
                const equilibrium &eq = (***gn).host; assert(eq.prod->size>0); assert(eq.reac->size<=0);
                if(sf.adjust(C,eq.prod))
                {
                    const double     xi = sf.xi;
                    for(const cnode *cn = eq.head();cn;cn=cn->next)
                    {
                        const component &a = ***cn; assert(a.nu>0);
                        const species   &s = *a;
                        const size_t     j = *s;
                        C[j] = max_of<double>( C[j] + a.nu * xi, 0.0);
                    }
                    sf.vanish(C);
                }
            }

        }




        void equalizer:: adjust(writable<double> &C,
                                const cluster    &cc)
        {
            adjust_reac(C,*(cc.genus->reac_only));
            adjust_prod(C,*(cc.genus->prod_only));
        }

        void equalizer:: adjust(writable<double> &C)
        {
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
            {
                adjust(C,*cc);
            }
        }


    }

}
