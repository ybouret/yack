
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/utils.hpp"
#include <iomanip>


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

        static const char * score_msg(const bool ok)   throw()
        {
            return ok ? "[succes]" : "[no gain]";
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
            YACK_XMLOG(xml,"\\___|cluster| = " << cc.size);
            YACK_XMLOG(xml," \\__|roaming| = " << cc.genus->roaming.size);
            YACK_XMLOG(xml,"  \\_|bounded| = " << cc.genus->bounded.size);
            YACK_XMLOG(xml,"check: " << cc.working);
            for(const anode *an = cc.working->head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;


        TRY_BALANCE:
            YACK_XMLOG(xml,"-------- gathering bounded balance");
            lead.release();
            const gathering    &eqs = (**this).lattice;
            for(const eq_node  *en  = cc.genus->bounded.head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                unsigned           flag = is_now_balanced;

                // interleaved scanning
                probe(reac,eq.reac->head,C0); if(reac.neg.size()) flag |= unbalanced_reac;
                probe(prod,eq.prod->head,C0); if(prod.neg.size()) flag |= unbalanced_prod;

                // check
                switch(flag)
                {
                    case unbalanced_both:
                        continue;

                    case unbalanced_reac: {
                        assert(prod.lim.size>0);
                        assert(reac.neg.size()>0);
                        assert(prod.neg.size()<=0);
                        reac.look_up(fade,prod);
                        coerce(fade.xi) = -fade.xi;
                        const bool ok   = score(C0,eq);
                        if(xml.verbose)
                            eqs.pad(*xml << eq.name,eq)
                            << balanced_msg[flag]  << reac.neg
                            << " | limited by prod: " << prod.lim
                            << " => " << fade << " => " << score_msg(ok) << std::endl;
                    } break;

                    case unbalanced_prod: {
                        assert(reac.lim.size>0);
                        assert(prod.neg.size()>0);
                        assert(reac.neg.size()<=0);

                        prod.look_up(fade,reac);
                        const bool ok = score(C0,eq);
                        if(xml.verbose)
                            eqs.pad(*xml << eq.name,eq)
                            << balanced_msg[flag] <<  prod.neg
                            << " | limited by reac: " << reac.lim
                            << " => " << fade << " => " << score_msg(ok) << std::endl;
                    } break;

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
            YACK_XMLOG(xml,"-------- status:");
            for(const eq_knot *node=lead.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(xml.verbose)
                {
                    eqs.pad(*xml << eq.name,eq) << " : " << std::setw(15) << gain[ei] << "@";
                    eq.display_compact(xml(), Cbal[ei]);
                    xml() << std::endl;
                }
            }
            
            for(const eq_squad *squad=cc.wing->head;squad;squad=squad->next)
            {

            }


            return false;
        }


        bool balancing:: score(const readable<double> &C0, const equilibrium &eq)
        {
            writable<double>          &C    = Cbal[*eq];
            const readable<criterion> &crit = (**this).crit;

            //------------------------------------------------------------------
            //
            // first pass: evaluate best concentration
            //
            //------------------------------------------------------------------

            iota::load(C,C0);

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
                        double      &c  = (C[j]=C0[j]+d);
                        if(c0>=0)    c  = max_of(c,0.0);
                        continue;
                }

            }

            //------------------------------------------------------------------
            //
            // second pass: numerical zero(s)
            //
            //------------------------------------------------------------------
            for(const sp_knot *sn=fade.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                const size_t   j  = *sp;
                C[j] = 0;
            }

            //------------------------------------------------------------------
            //
            // third pass: get improvement on negative conserved values
            //
            //------------------------------------------------------------------
            xadd.ldz();

            for(const cnode *cn=eq.head();cn;cn=cn->next)
            {
                const size_t j = *****cn;
                switch(crit[j])
                {
                    case unbounded:
                    case spectator:
                        continue;

                    case conserved:
                        const double c0 = C0[j];
                        if(c0<0) {
                            xadd.push(-c0);
                            const double c= C[j];
                            if(c<0)
                                xadd.push(c);
                        }
                }
            }

            const double g = xadd.get();
            if(g>0)
            {
                lead << &eq;
                gain[*eq] = g;
                return true;
            }
            else
            {
                return false;
            }
        }
        
    }
}

