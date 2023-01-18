#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical
    {

        sq_repo::  sq_repo(const size_t n) : sq_repo_(n) {}
        sq_repo:: ~sq_repo() throw() {}


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
        wall( *this ),
        reac(mx,*this),
        prod(mx,*this),
        eqdb(),
        pick(cs.N),
        Ceqz(cs.L,cs.L?cs.M:0),
        gain(cs.L,0.0),
        xadd()
        {
        }

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
            adjust_reac(C,*(cc.replica->genus->reac_only),xml);
            adjust_prod(C,*(cc.replica->genus->prod_only),xml);
        }

        void equalizer:: adjust(writable<double> &C, const xmlog &xml)
        {
            YACK_XMLSUB(xml, "equalizer::adjust" );
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
                adjust(C,*cc,xml);
        }

        
        void equalizer:: comply(writable<double> &C, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"equalizer::comply");
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

        bool equalizer:: is_complying(const squad &sq) const throw()
        {
            for(const eq_node *node=sq.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if( !eqdb.search(&eq) ) return false;
            }
            return true;
        }

        


        void equalizer:: comply(writable<double> &C,
                                const cluster    &cc,
                                const xmlog      &xml)
        {

            YACK_XMLSUB(xml,"equalizer::cluster");

            //------------------------------------------------------------------
            //
            // detect negative conserved species
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- inspecting negative species");
            bool run = false;
            for(const sp_gnode *sn = cc.replica->breed->conserved->head;sn;sn=sn->next)
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
            {
                YACK_XMLOG(xml, "-- all good!");
                return;
            }

            //------------------------------------------------------------------
            //
            // compute all balancing equilibria
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- looking for balancing equilibira");
            eqdb.free();
            for(const eq_node *node = (*(cc.replica->genus->balancing)).head ;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const status       st = probe(C,eq);

                switch(st)
                {
                    case balanced: continue;
                    case bad_reac: {
                        YACK_XMLSUB(xml,eq.name);
                        YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        eqdb.ensure(&comply_reac(C,eq,xml));
                    } break;

                    case bad_prod: {
                        YACK_XMLSUB(xml,eq.name);
                        YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        eqdb.ensure(&comply_prod(C,eq,xml));
                    } break;

                    case bad_both:
                        //YACK_XMLOG(xml, "-- " << status_text(st) << " --" );
                        //YACK_XMLOG(xml, " |_amending reac: " << reac.amending);
                        //YACK_XMLOG(xml, " |_limiting prod:  " << prod.limiting);
                        //YACK_XMLOG(xml, " |_amending prod: " << prod.amending);
                        //YACK_XMLOG(xml, " |_limiting reac:  " << reac.limiting);
                        break;

                }
            }

            if( eqdb->size <= 0)
            {
                std::cerr << "stalled..." << std::endl;
                exit(0);
            }

            YACK_XMLOG(xml, "-- collecting balancing squads");
            pick.clear();
            for(const squad *sq=cc.wing->head;sq;sq=sq->next)
            {
                if(is_complying(*sq))
                {
                    pick << *sq;
                }
            }
            std::cerr << "pick=" << pick << std::endl;
            assert(pick.size>0);
            const sq_node *Best = pick.head;
            double         Gain = gain_of(***Best);
            YACK_XMLOG(xml, " (+) " << std::setw(15) << Gain << " @" << ***Best);
            for(const sq_node *node=Best->next;node;node=node->next)
            {
                const double temp = gain_of(***node);
                const bool   ok   = temp>Gain;
                YACK_XMLOG(xml, (ok?" (+) " : " (-) ") << std::setw(15) << temp << " @" << ***node );
                if(ok) {
                    Gain = temp;
                    Best = node;
                }
            }
            YACK_XMLOG(xml, " (*) " << std::setw(15) << Gain << " @" << ***Best);



        }

        double equalizer:: gain_of(const squad &sq)
        {
            xadd.ldz();
            for(const eq_node *node=sq.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                xadd.push( gain[*eq] );
            }
            return xadd.get();
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
                                       const readable<double> &C0,
                                       const equilibrium      &eq,
                                       const xmlog            &xml)
        {
            const size_t      ei = *eq;
            writable<double> &Ci = Ceqz[ei];
            const double      xx = F.xi;

            iota::load(Ci,C0);

            for(const cnode *cn = eq.head(); cn; cn=cn->next)
            {
                const component &cm = ***cn;
                const species   &sp = *cm;
                const size_t      j = *sp;
                Ci[j] += cm.nu * xx;
#if 0
                const double     dc = cm.nu * xx;
                const double     c0 = Ci[j];
                const double     c1 = Ci[j] += dc;
                if( (*cs.fixed)[j] )
                {
                    xadd.push(  min_of(c1,0.0) );
                    xadd.push( -min_of(c0,0.0) );
                }
#endif
            }
            F.vanish(Ci);

            const readable<bool> &used = *cs.fixed;
            xadd.ldz();
            for(const cnode *cn = eq.head(); cn; cn=cn->next)
            {
                const component &cm = ***cn;
                const species   &sp = *cm;
                const size_t      j = *sp;
                if(used[j])
                {
                    xadd.push(  min_of(Ci[j],0.0) );
                    xadd.push( -min_of(C0[j],0.0) );
                }
            }


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
