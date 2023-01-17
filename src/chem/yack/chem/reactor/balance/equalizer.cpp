#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical
    {

        equalizer:: er_type:: er_type(const size_t n) : er_type_(n) {}
        equalizer:: er_type:: ~er_type() throw() {}

        std::ostream & operator<<( std::ostream &os, const equalizer::er_type &self )
        {
            os << '{';
            const eq_node *node = self.head;
            if(node) {
                os << '<' << (***node).name << '>';
                for(node=node->next;node;node=node->next)
                {
                    os << ", " <<'<' << (***node).name << '>';
                }
            }
            os << '}';
            return os;
        }

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
        used(cs.N),
        Ceqz(cs.L,cs.L?cs.M:0),
        gain(cs.L,0.0)
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

            // detect negative conserved species
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

            // compute all balancing equilibria
            used.clear();
            for(const eq_node *node = (*(cc.genus->balancing)).head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const status       st = probe(C,eq);

                switch(st)
                {
                    case balanced: continue;
                    case bad_reac: {
                        YACK_XMLSUB(xml,eq.name);
                        YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        used << comply_reac(C,eq,xml);
                    } break;

                    case bad_prod: {
                        YACK_XMLSUB(xml,eq.name);
                        YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        used << comply_prod(C,eq,xml);
                    } break;

                    case bad_both:
                        YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        //YACK_XMLOG(xml, " |_amending reac: " << reac.amending);
                        //YACK_XMLOG(xml, " |_limiting prod:  " << prod.limiting);
                        //YACK_XMLOG(xml, " |_amending prod: " << prod.amending);
                        //YACK_XMLOG(xml, " |_limiting reac:  " << reac.limiting);
                        break;

                }
            }

            if( used.size <= 0)
            {
                std::cerr << "stalled..." << std::endl;
                exit(0);
            }
            std::cerr << "used=" << used << std::endl;
            

        }


        const equilibrium & equalizer:: comply_prod(const readable<double> &C,
                                                    const equilibrium      &eq,
                                                    const xmlog            &xml)
        {
            YACK_XMLOG(xml, " |_amending prod: "  << prod.amending); assert(prod.amending.size());
            YACK_XMLOG(xml, " |_limiting reac:  " << reac.limiting);

            frontier fwd(*this);
            locate_single_fence(fwd,reac.limiting,prod.amending,xml);
            comply_move(fwd,C,eq,xml);
            return eq;
        }


        const equilibrium & equalizer:: comply_reac(const readable<double> &C,
                                                    const equilibrium      &eq,
                                                    const xmlog            &xml)
        {
            YACK_XMLOG(xml, " |_amending reac: "  << reac.amending); assert(reac.amending.size());
            YACK_XMLOG(xml, " |_limiting prod:  " << prod.limiting);

            frontier rev(*this);
            locate_single_fence(rev,prod.limiting,reac.amending,xml);
            rev.xi = -rev.xi;
            comply_move(rev,C,eq,xml);
            return eq;
        }

        void  equalizer::  comply_move(const frontier         &F,
                                       const readable<double> &C,
                                       const equilibrium      &eq,
                                       const xmlog            &xml)
        {
            const size_t      ei = *eq;
            writable<double> &Ci = Ceqz[ei];
            const double      xx = F.xi;

            iota::load(Ci,C);
            xadd.ldz();

            for(const cnode *cn = eq.head(); cn; cn=cn->next)
            {
                const component &cm = ***cn;
                const species   &sp = *cm;
                const size_t      j = *sp;
                const double     dc = cm.nu * xx;
                const double     c0 = Ci[j];
                const double     c1 = Ci[j] += dc;
                xadd.push(  min_of(c1,0.0) );
                xadd.push( -min_of(c0,0.0) );
            }
            F.vanish(Ci);
            gain[ei] = xadd.get();
            if(xml.verbose)
            {
                eq.display_compact(*xml << " | ",Ci,*cs.fixed) << std::endl;
                *xml <<" | gain = " << std::setw(15) << gain[ei] << std::endl;
            }
        }

        void equalizer:: locate_single_fence(frontier        &sf,
                                             const frontier  &limiting,
                                             const frontiers &amending,
                                             const xmlog     &xml)
        {
            static const char pfx[] = " | --> ";

            YACK_XMLOG(xml, pfx  << "locate single fence in " << amending);
            assert(amending.size()>0);
            assert(amending.are_increasing());
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

                assert(xx>amending.front().xi);
                assert(xx<amending.back().xi);

                size_t jlo = 1;
                size_t jup = n;

            LOOK_UP:
                assert(xx>amending[jlo].xi);
                assert(xx<amending[jup].xi);
                if(jup-jlo<=1)
                {
                    sf = amending[jlo];
                    YACK_XMLOG(xml,pfx << "solve core  " << sf << " #" << jlo) ;
                    return;
                }

                const size_t     jnx = jlo+1; assert(jnx<=jup);
                const frontier  &af  = amending[jnx];
                switch( __sign::of(xx,af.xi) )
                {
                    case negative:
                        sf = amending[jlo];
                        YACK_XMLOG(xml,pfx << "solve core  " << sf << " #" << jlo) ;
                        return;

                    case __zero__:
                        (sf = limiting) += af;
                        YACK_XMLOG(xml,pfx << "match core  " << sf << " #" << jnx);
                        break;

                    case positive:
                        jlo=jnx;
                        goto LOOK_UP;
                }




            }

        }


    }

}
