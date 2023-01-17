#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical
    {

        const char * equalizer:: status_text(const status s) throw()
        {
            switch(s)
            {
                case balanced: return "balanced";
                case bad_reac: return "bad_reac";
                case bad_prod: return "bad_prod";
                case bad_both: return "bad_both";
            }
            return yack_unknown;
        }

        equalizer:: ~equalizer() throw() {}


        equalizer:: equalizer(const reactor &_) :
        sp_fund( new sp_bank() ),
        cs(_),
        mx(cs.max_actors()),
        sf( *this ),
        reac(mx),
        prod(mx),
        Ceqz(cs.L,cs.L?cs.M:0)
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

        
        void equalizer:: comply(writable<double> &C)
        {
            const xmlog xml("[equalizer]",std::cerr,reactor::verbose);
            YACK_XMLSUB(xml,"comply");
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
                comply(C,*cc,xml);
        }

        equalizer::status equalizer:: probe(const readable<double> &C, const equilibrium &eq)
        {
            const bool amend_reac = reac.probe(C,eq.reac,*this,*cs.fixed);
            const bool amend_prod = prod.probe(C,eq.prod,*this,*cs.fixed);

            if(amend_reac)
            {
                if(amend_prod)
                {
                    return bad_both;
                }
                else
                {
                    return bad_reac;
                }
            }
            else
            {
                if(amend_prod)
                {
                    return bad_prod;
                }
                else
                {
                    return balanced;
                }
            }


        }


        void equalizer:: comply(writable<double> &C,
                                const cluster    &cc,
                                const xmlog      &xml)
        {

            YACK_XMLSUB(xml,"cluster");

            bool run = false;
            for(const sp_gnode *sn = cc.breed->conserved->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                const double   c = C[*s];
                if(c<0)
                {
                    run = true;
                    if(xml.verbose) {
                        cs.lib.pad(*xml << " (-) " << '[' << s.name  << ']',s) << " = " << std::setw(15) << c << std::endl;
                    }
                }
            }

            if(!run)
                return;

        //CYCLE:
            // compute all balancing equilibria
            for(const eq_node *node = (*(cc.genus->balancing)).head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const status       st = probe(C,eq);
                YACK_XMLSUB(xml,eq.name);
                YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                switch(st)
                {
                    case balanced: continue;
                    case bad_reac:
                        YACK_XMLOG(xml, " |_amending reac: " << reac.amending);
                        YACK_XMLOG(xml, " |_limiting prod: " << prod.limiting);
                        break;

                    case bad_prod:
                        comply_prod(C,eq,xml);
                        break;

                    case bad_both:
                        YACK_XMLOG(xml, " |_amending reac: " << reac.amending);
                        YACK_XMLOG(xml, " |_limiting prod: " << prod.limiting);
                        YACK_XMLOG(xml, " |_amending prod: " << prod.amending);
                        YACK_XMLOG(xml, " |_limiting reac: " << reac.limiting);
                        break;

                }

            }
        }
        

        void equalizer:: comply_prod(const readable<double> &C,
                                     const equilibrium      &eq,
                                     const xmlog            &xml)
        {
            YACK_XMLOG(xml, " |_amending prod: " << prod.amending);
            YACK_XMLOG(xml, " |_limiting reac: " << reac.limiting);

            const size_t      ei = *eq;
            writable<double> &Ci = Ceqz[ei];
            iota::load(Ci,C);
            const frontier &mx = reac.limiting.front();
            const double    xx = mx.xi;
            YACK_XMLOG(xml, " max extent = " << mx);

        }

    }

}
