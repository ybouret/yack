
#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical
    {

        void equalizer:: comply(writable<double> &C, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"equalizer::comply");
            for(const cluster *cc=cs.linked->head;cc;cc=cc->next)
                comply(C,*cc,xml);
        }

        equalizer::status equalizer:: find_status(const readable<double> &C, const equilibrium &eq)
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

        bool equalizer:: is_solution(const squad &sq) const throw()
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

            YACK_XMLSUB(xml,"equalize::cluster");


            {
                //--------------------------------------------------------------
                //
                YACK_XMLSUB(xml,"equalize::look_for_negative_species");
                //
                //--------------------------------------------------------------
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
                    YACK_XMLOG(xml, "--> all species are OK");
                    return;
                }
            }


            {
                //--------------------------------------------------------------
                //
                YACK_XMLSUB(xml,"equalize::balancing_equilibri[um|a]");
                //
                //--------------------------------------------------------------

                eqdb.free();
                for(const eq_node *node = (*(cc.replica->genus->balancing)).head ;node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const status       st = find_status(C,eq);

                    switch(st)
                    {
                        case balanced: continue;
                        case bad_reac: {
                            YACK_XMLSUB(xml,eq.name);
                            YACK_XMLOG(xml, "==> " << status_text(st) << " <==" );
                            eqdb.ensure(&comply_reac(C,eq,xml));
                        } break;

                        case bad_prod: {
                            YACK_XMLSUB(xml,eq.name);
                            YACK_XMLOG(xml, "==> " << status_text(st) << " <==" );
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
            }


            {
                //--------------------------------------------------------------
                //
                YACK_XMLSUB(xml,"equalize::collect_suitable_squad[s]");
                //
                //--------------------------------------------------------------
                pick.clear();
                for(const squad *sq=cc.wing->head;sq;sq=sq->next)
                {
                    if(is_solution(*sq))
                    {
                        pick << *sq;
                        YACK_XMLOG(xml, "--> " << *sq);
                    }
                }
                assert(pick.size>0);
            }


            {
                //--------------------------------------------------------------
                //
                YACK_XMLSUB(xml,"equalize::query_the_balancing_squad");
                //
                //--------------------------------------------------------------
                const sq_node *Best = pick.head;
                double         Gain = gained_thru(***Best);
                YACK_XMLOG(xml, " (+) " << std::setw(15) << Gain << " @" << ***Best);
                for(const sq_node *node=Best->next;node;node=node->next)
                {
                    const double temp = gained_thru(***node);
                    const bool   ok   = temp>Gain;
                    YACK_XMLOG(xml, (ok?" (+) " : " (-) ") << std::setw(15) << temp << " @" << ***node );
                    if(ok) {
                        Gain = temp;
                        Best = node;
                    }
                }
                YACK_XMLOG(xml, " (*) " << std::setw(15) << Gain << " @" << ***Best << " <--");
                make_better(C,***Best,xml);
            }
        }



        double equalizer:: gained_thru(const squad &sq)
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
            locate_wall(fwd,reac.limiting,prod.amending,xml);
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
            locate_wall(rev,prod.limiting,reac.amending,xml);
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

            //------------------------------------------------------------------
            // compute new concentrations
            //------------------------------------------------------------------
            iota::load(Ci,C0);
            for(const cnode *cn = eq.head(); cn; cn=cn->next)
            {
                const component &cm = ***cn;
                const species   &sp = *cm;
                const size_t      j = *sp;
                Ci[j] += cm.nu * xx;
            }
            F.vanish(Ci);

            //------------------------------------------------------------------
            // compute gain
            //------------------------------------------------------------------
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

        void equalizer:: make_better(writable<double> &C0,
                                     const squad      &sq,
                                     const xmlog      &xml)
        {
            YACK_XMLSUB(xml,"equalize::make_improved_phase_space");
            YACK_XMLOG(xml, "using " << sq);
            YACK_XMLOG(xml, "autonomous: " << sq.autonomous);
            YACK_XMLOG(xml, "mutualized: " << sq.mutualized);
            
            for(const eq_node *node=sq.head;node;node=node->next)
            {
                const equilibrium &      eq = ***node;
                const size_t             ei = *eq;
                const readable<double> & Ci = Ceqz[ei];
                eq.display_compact( sq.pad(std::cerr << eq.name,eq) << " : ",Ci);
                std::cerr << std::endl;
            }
        }

    }

}


