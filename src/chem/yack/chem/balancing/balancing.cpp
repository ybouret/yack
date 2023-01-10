
#include "yack/chem/balancing/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/utils.hpp"
#include <iomanip>
#include "yack/system/imported.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        balancing:: ~balancing() throw()
        {
        }

        balancing:: balancing(const nexus &usr, const xmlog &out) :
        authority<const nexus>(usr),
        spIO( new sp_pool() ),
        eqIO( new eq_pool() ),
        reac(usr.M,spIO),
        prod(usr.M,spIO),
        Cres(usr.M),
        Cbal(usr.L,usr.L>0?usr.M:0),
        fade(spIO),
        lead(eqIO),
        gain( usr.L ),
        xadd(),
        xml(out)
        {}

        bool balancing:: operator()(writable<double> &C0)
        {
            bool result = true;
            for(const cluster *cls= (**this).wired.head;cls;cls=cls->next)
            {
                if(!balance(C0,*cls)) {
                    result = false;
                }
            }
            return result;
        }

        
        void balancing:: probe(equalizer &eqz, const actor *a, const readable<double> &C)
        {
            const readable<criterion> &crit = (**this).crit;
            eqz.destroy();
            for(;a;a=a->next)
            {
                const species &s = **a;
                const size_t   j = *s; if(conserved!=crit[j]) continue;
                const double   c = C[j];
                if(c<0)
                {
                    eqz.neg.upgrade( (-c)/a->nu,s );
                }
                else
                {
                    eqz.lim.upgrade( c/a->nu,s );
                }
            }
        }



        static const unsigned is_now_balanced  = 0x00;
        static const unsigned unbalanced_reac  = 0x01;
        static const unsigned unbalanced_prod  = 0x02;
        static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;
        
        static const char * const balanced_msg[] =
        {
            " [+] ",
            " [<] ",
            " [>] ",
            " [-] "
        };



        static inline bool squad_includes_lead(const eq_squad &squad,
                                               const eq_repo  &lead) throw()
        {
            for(const eq_node *en=squad.head;en;en=en->next)
            {
                if(!lead.contains(**en)) return false;
            }
            return true;
        }

        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            //------------------------------------------------------------------
            //
            // check species in this cluster
            //
            //------------------------------------------------------------------
            YACK_XMLSUB(xml, "balancing:cluster");
            YACK_XMLOG(xml,"\\_____|cluster| = " << cc.size);
            YACK_XMLOG(xml," \\____|roaming| = " << cc.genus->roaming.size);
            YACK_XMLOG(xml,"  \\___|bounded| = " << cc.genus->bounded.size);
            YACK_XMLOG(xml,"   \\__unbridled = " << cc.genus->unbridled);
            YACK_XMLOG(xml,"    \\_regulated = " << cc.genus->regulated);

        CHK_BALANCE:
            for(const sp_node *an = cc.genus->committed.head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;


        TRY_BALANCE:
            YACK_XMLOG(xml,"-------- bounded limits --------");
            const gathering           &eqs  = (**this).lattice;
            const readable<criterion> &crit = (**this).crit;
            if(cc.genus->bounded.size)
            {
                lead.release();
                for(const eq_node  *en  = cc.genus->bounded.head;en;en=en->next)
                {
                    const equilibrium &eq   = **en;
                    unsigned           flag = is_now_balanced;

                    //--------------------------------------------------------------
                    //
                    // interleaved scanning
                    //
                    //--------------------------------------------------------------
                    probe(reac,eq.reac->head,C0); if(reac.neg.size()) flag |= unbalanced_reac;
                    probe(prod,eq.prod->head,C0); if(prod.neg.size()) flag |= unbalanced_prod;

                    //--------------------------------------------------------------
                    //
                    // analyze what is possible
                    //
                    //--------------------------------------------------------------
                    switch(flag)
                    {
                            //------------------------------------------------------
                            // blocked, ok or not, depending on c-laws
                            //------------------------------------------------------
                        case unbalanced_both:
                            continue;

                            //------------------------------------------------------
                            // negative reac(s), positive prod
                            //------------------------------------------------------
                        case unbalanced_reac:
                            assert(prod.lim.size>0);
                            assert(reac.neg.size()>0);
                            assert(prod.neg.size()<=0);

                            reac.look_up(fade,prod);       // analyze
                            fade.reverse();                // adjust
                            score(C0,eq);                  // score

                            if(xml.verbose)
                                eqs.pad(*xml << eq.name,eq)
                                << balanced_msg[flag]  << reac.neg
                                << " | limited by prod: " << prod.lim
                                << " => " << fade << std::endl;
                            break;

                            //------------------------------------------------------
                            // negative prod(s), positive reac
                            //------------------------------------------------------
                        case unbalanced_prod:
                            assert(reac.lim.size>0);
                            assert(prod.neg.size()>0);
                            assert(reac.neg.size()<=0);

                            prod.look_up(fade,reac); // analyze
                            score(C0,eq);            // score
                            if(xml.verbose)
                                eqs.pad(*xml << eq.name,eq)
                                << balanced_msg[flag] <<  prod.neg
                                << " | limited by reac: " << reac.lim
                                << " => " << fade << std::endl;
                            break;

                        default:
                            assert(0==flag);
                            continue;
                    }
                }

                if(lead.size<=0)
                {
                    std::cerr << "stuck..." << std::endl;
                    exit(0);
                }

                if(xml.verbose)
                {
                    *xml << "-------- bounded status --------" << std::endl;
                    for(const eq_knot *node=lead.head;node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        eqs.pad(*xml << eq.name,eq) << " : " << std::setw(15) << gain[ei] << "@";
                        eq.display_compact(xml(), Cbal[ei]);
                        xml() << std::endl;
                    }
                }

                YACK_XMLOG(xml,"-------- bounded select --------");

                double          gain = 0;
                const eq_squad *best = champ(cc.wing->head,gain);
                YACK_XMLOG(xml, " [*] " << std::setw(15) << gain << " @" << *best);

                std::cerr << C0 <<  " @C0" << std::endl;
                iota::load(Cres,C0);
                for(const eq_node *node=best->head;node;node=node->next)
                {
                    const equilibrium      &eq = **node;
                    const readable<double> &Ci = Cbal[*eq];
                    std::cerr << Ci << " @" << eq.name << std::endl;
                    for(const cnode *cn=eq.head();cn;cn=cn->next)
                    {
                        const species &s = ****cn;
                        const size_t   j = *s;
                        switch(crit[j])
                        {
                            case conserved:
                                Cres[j] = Ci[j];
                                continue;

                            default:
                                continue;
                        }
                    }
                }
                std::cerr << Cres << " @C1" << std::endl;


                for(const sp_node *sn=cc.genus->unbridled.head;sn;sn=sn->next)
                {
                    const species &s = **sn;
                    const size_t   j = *s;
                    const double   c0 = C0[j];
                    xadd.ldz();
                    xadd.push(c0);
                    for(const eq_node *node=best->head;node;node=node->next)
                    {
                        const equilibrium      &eq = **node;
                        const readable<double> &Ci = Cbal[*eq];
                        xadd.push( Ci[j] );
                        xadd.push( -c0 );
                    }

                    Cres[j] = xadd.get();
                }
                iota::save(C0, Cres);
                std::cerr << C0 << " @C0" << std::endl;
            }

            clean(C0,cc);

            goto CHK_BALANCE;
        }

        void balancing:: clean(writable<double> &C0, const cluster &cc)
        {
            for(const eq_node  *en  = cc.genus->roaming.head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                switch(eq.kind)
                {
                    case undefined:
                    case both_ways:
                        continue;

                    case part_only:
                        assert(eq.prod->size>0);
                        assert(eq.reac->size<=0);
                        prod.lim.ltz(C0,eq.prod->head);
                        std::cerr << "prod: " << prod.lim << std::endl;
                        prod.lim.reverse();
                        prod.lim.mov(C0,eq.prod->head);
                        prod.lim.ldz(C0);
                        std::cerr << "C0=" << C0 << std::endl;
                        continue;

                    case join_only:
                        assert(eq.reac->size>0);
                        assert(eq.prod->size<=0);
                        reac.lim.ltz(C0,eq.reac->head);
                        std::cerr << "reac: " << reac.lim << std::endl;
                        reac.lim.reverse();
                        reac.lim.mov(C0,eq.reac->head);
                        reac.lim.ldz(C0);
                        std::cerr << "C0=" << C0 << std::endl;
                        continue;
                }

            }
        }

        const eq_squad * balancing:: champ(const eq_squad *squad, double &Gain)
        {
            //------------------------------------------------------------------
            //
            // find first squad
            //
            //------------------------------------------------------------------
            assert(NULL!=squad);
            while( !squad_includes_lead(*squad,lead) )
            {
                squad = squad->next;
                if(!squad) throw imported::exception("balancing","no equilibria squad matching balancing lead!!");
            }
            const eq_squad *Best = squad;
            Gain                 = total(*Best);
            YACK_XMLOG(xml, " [+] " << std::setw(15) << Gain << " @" << *Best);

            //------------------------------------------------------------------
            //
            // find better squad
            //
            //------------------------------------------------------------------
            for(squad=squad->next;squad;squad=squad->next)
            {
                if(!squad_includes_lead(*squad,lead) ) continue;;
                const double Temp = total(*squad);
                const bool   ok   = Temp>Gain;
                YACK_XMLOG(xml, (ok?" [+] " : " [-] ") << std::setw(15) << Temp << " @" << *squad);
                if(ok) {
                    Best = squad;
                    Gain = Temp;
                }
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
            return Best;
        }


        double balancing:: total(const eq_squad &squad)
        {
            xadd.ldz();
            for(const eq_node *node=squad.head;node;node=node->next)
            {
                const equilibrium &eq = **node;
                xadd.push( gain[ *eq ]);
            }
            return xadd.get();
        }


        void balancing:: score(const readable<double> &C0, const equilibrium &eq)
        {
            writable<double>          &C    = Cbal[*eq];
            const readable<criterion> &crit = (**this).crit;

            //------------------------------------------------------------------
            //
            // first pass: evaluate best concentration
            //
            //------------------------------------------------------------------
            iota::load(C,C0);

            xadd.ldz();
            const double xi = fade.xi;
            for(const cnode *cn=eq.head();cn;cn=cn->next)
            {
                const component &a = ***cn;
                const species   &s = *a;
                const size_t     j = *s;
                const double     d = a.nu * xi;

                switch( crit[j] )
                {
                    case unbounded:
                        //------------------------------------------------------
                        // move freely
                        //------------------------------------------------------
                        C[j] = C0[j] + d;
                        continue;

                    case spectator:
                        //------------------------------------------------------
                        // never get here!
                        //------------------------------------------------------
                        continue;

                    case conserved:
                        //------------------------------------------------------
                        // a positive value will never become negative
                        //------------------------------------------------------
                        const double c0 = C0[j];
                        if(c0<0)
                        {
                            double &c =(C[j]=c0+d);
                            if(c<0) {
                                xadd.push(d);
                            }
                            else
                            {
                                xadd.push(-c0);
                            }
                        }
                        else
                        {
                            C[j]  = max_of(c0+d,0.0);
                        }
                        continue;
                }

            }

            //------------------------------------------------------------------
            //
            // second pass: numerical zero(s)
            //
            //------------------------------------------------------------------
            fade.ldz(C);

            //------------------------------------------------------------------
            //
            // update lead and gain
            //
            //------------------------------------------------------------------
            lead << &eq;
            gain[*eq] = xadd.get();

        }
        
    }
}
