#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/system/imported.hpp"
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
        reac(mx,*this),
        prod(mx,*this),
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
                    case bad_reac: comply_reac(C,eq,xml); break;
                    case bad_prod: comply_prod(C,eq,xml); break;

                    case bad_both:
                        YACK_XMLOG(xml, " |_amending reac: " << reac.amending);
                        YACK_XMLOG(xml, " |_limiting prod:  " << prod.limiting);
                        YACK_XMLOG(xml, " |_amending prod: " << prod.amending);
                        YACK_XMLOG(xml, " |_limiting reac:  " << reac.limiting);
                        break;

                }

            }
        }


        void equalizer:: locate_single_fence(frontier        &sf,
                                             const frontier  &limiting,
                                             const frontiers &amending,
                                             const xmlog     &xml)
        {
            static const char pfx[] = " | --> ";

            assert(amending.size()>0);
            sf.free();
            if(limiting.size<=0)
            {
                //--------------------------------------------------------------
                //
                // take the biggest amending!
                //
                //--------------------------------------------------------------
                sf =  amending.back();
                YACK_XMLOG(xml, pfx << "greatest at " << sf);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // generic case: test w.r.t front amending
                //
                //--------------------------------------------------------------
                const size_t n  =  amending.size();
                const double xx =  limiting.xi;
                switch( __sign::of(xx,amending.front().xi) )
                {
                    case negative:
                        sf = limiting;
                        YACK_XMLOG(xml,pfx << "best effort " << sf << " [won't solve]");
                        return;

                    case __zero__:
                        (sf = limiting) += amending.front();
                        YACK_XMLOG(xml,pfx << "match first " << sf);
                        return;

                    case positive:
                        if(n<=1)
                        {
                            sf =  amending.front();
                            YACK_XMLOG(xml,pfx << "solve last  " << sf << " [unique species]");
                            return;
                        }
                        break;
                }

                assert(amending.size()>=2);

                //--------------------------------------------------------------
                //
                // generic case: test w.r.t back amending
                //
                //--------------------------------------------------------------
                switch( __sign::of(xx,amending.back().xi) )
                {
                    case positive:
                        sf = amending.back();
                        YACK_XMLOG(xml,pfx << "solve last  " << sf);
                        return;

                    case __zero__:
                        (sf = limiting) +=  amending.back();
                        YACK_XMLOG(xml,pfx << "match last  " << sf);
                        return;

                    case negative:
                        break;
                }

                for(size_t i=2;i<n;++i)
                {
                    const frontier &af = amending[i];

                    switch( __sign::of(xx,af.xi) )
                    {
                        case negative:
                            sf = amending[i-1];
                            YACK_XMLOG(xml,pfx << "solve core  " << sf << " #" << i-1) ;
                            break;

                        case __zero__:
                            (sf = limiting) += af;
                            YACK_XMLOG(xml,pfx << "match core  " << sf << " #" << i);
                            break;

                        case positive:
                            continue;
                    }

                }

                if( !sf.size ) throw imported::exception("equalizer::locate","corrupted look up!!");



            }

        }

        void equalizer:: comply_prod(const readable<double> &C,
                                     const equilibrium      &eq,
                                     const xmlog            &xml)
        {
            YACK_XMLOG(xml, " |_amending prod: "  << prod.amending); assert(prod.amending.size());
            YACK_XMLOG(xml, " |_limiting reac:  " << reac.limiting);

            const size_t      ei = *eq;
            writable<double> &Ci = Ceqz[ei];
            iota::load(Ci,C);
            locate_single_fence(sf,reac.limiting,prod.amending,xml);
        }


        void equalizer:: comply_reac(const readable<double> &C,
                                     const equilibrium      &eq,
                                     const xmlog            &xml)
        {
            YACK_XMLOG(xml, " |_amending reac: "  << reac.amending); assert(reac.amending.size());
            YACK_XMLOG(xml, " |_limiting prod:  " << prod.limiting);

            const size_t      ei = *eq;
            writable<double> &Ci = Ceqz[ei];
            iota::load(Ci,C);
            locate_single_fence(sf,prod.limiting,reac.amending,xml);
        }

    }

}
