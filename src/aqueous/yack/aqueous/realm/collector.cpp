#include "yack/aqueous/realm/collector.hpp"
#include "yack/aqueous/realm/claw/custodian.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace aqueous
    {

        
        collector:: ~collector() noexcept
        {

        }

        collector:: collector(const size_t n, const size_t m) :
        collecting::caches(),
        collector_(n,*this),
        balanced(eqp),
        solvable(eqp),
        weakened(eqp),
        singular(eqp),
        solo(zlp,spp),
        gain(n,0),
        Cend(m,0),
        Cbal(n,m),
        xadd()
        {
        }

        void collector:: initialize() noexcept
        {
            balanced.clear();
            solvable.clear();
            weakened.clear();
            singular.clear();
            gain.ld(-1);
        }


        namespace
        {
            static inline
            double compute_balanced(cameo::add<double>    &xadd,
                                    writable<double>       &Cb,
                                    const components       &eq,
                                    const readable<double> &C0,
                                    const readable<bool>   &R,
                                    const zlimit           &zl) noexcept
            {

                //--------------------------------------------------------------
                //
                // load all phase space
                //
                //--------------------------------------------------------------
                math::iota::load(Cb,C0);

                //--------------------------------------------------------------
                //
                // apply extent
                //
                //--------------------------------------------------------------
                const double xi = zl.extent;
                for(const cnode *node=eq->head;node;node=node->next)
                {
                    const component &cc = ***node;
                    const species   &sp = *cc;
                    const size_t     j  = sp.indx[top_level];
                    Cb[j] += xi * cc.nu;
                }

                //--------------------------------------------------------------
                //
                // use vanishing
                //
                //--------------------------------------------------------------
                zl.nullify(Cb);

                //--------------------------------------------------------------
                //
                // deduce gain
                //
                //--------------------------------------------------------------
                xadd.free();
                for(const cnode *node=eq->head;node;node=node->next)
                {
                    const component &cc = ***node;
                    const species   &sp = *cc;
                    const size_t     j  = sp.indx[top_level]; if( !R[j] ) continue;
                    const double     c0 = C0[j];
                    if(c0<0)
                    {
                        xadd.push( -c0 );
                        xadd.push( min_of(Cb[j],0.0) );
                    }
                    else
                    {
                        assert(Cb[j]>=0);
                    }
                }
                return xadd.sum();

            }



            static inline void display_gains(const xmlog            &xml,
                                             const readable<double> &Gain,
                                             const gathering        &fmt,
                                             const eq_node          *en)
            {

                YACK_XMLSUB(xml, "gain_per_equilibrium");
                for(;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = eq.indx[cat_level];
                    fmt.pad(*xml << eq, eq) << " : " << Gain[ei] << std::endl;
                }

            }



        }


        double collector:: combined(const cluster &clan)
        {
            xadd.free();
            for(const eq_node *en = clan.head; en; en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[cat_level];
                const double       g  = gain[ei];assert(g>=0);
                xadd.push(g);
            }
            return xadd.sum();
        }

        const cluster & collector:: find_opt(const partition        &part,
                                             const eq_repo          &zeqs,
                                             const xmlog            &xml )
        {
            YACK_XMLSUB(xml, "gain_for_each_subset");

            assert(part.size>0);
            assert(zeqs.size>0);
            const cluster *win = part.head;
            double         opt = 0;

        FIRST_WINNER:
            if( !win->is_subset_of(zeqs) )
            {
                win = win->next;
                if(!win) throw imported::exception("aqueous::collector","no initial subset: corrupted code");
                goto FIRST_WINNER;
            }
            assert(NULL!=win);
            opt = combined(*win);

            YACK_XMLOG(xml, "(+) " << std::setw(15) << opt << " @" << *win);
            for(const cluster *cls=win->next;cls;cls=cls->next)
            {
                if(!cls->is_subset_of(zeqs)) continue;
                const double tmp = combined(*cls);
                if(tmp>opt)
                {
                    opt = tmp;
                    win = cls;
                    YACK_XMLOG(xml, "(+) " << std::setw(15) << opt << " @" << *win);
                }
                else
                {
                    YACK_XMLOG(xml, "(-) " << std::setw(15) << tmp << " @" << *cls);
                }
            }

            YACK_XMLOG(xml, "--> " << std::setw(15) << opt << " @" << *win);
            return *win;
        }


        bool collector:: dispatch(const equilibrium &eq, const shift_status st)
        {
            switch(st)
            {
                case shift_success: solvable << eq; return true;
                case shift_partial: weakened << eq; return true;
                case shift_blocked: singular << eq; break;
            }
            return false;
        }


        void collector:: balance(const xmlog            &xml,
                                 const domain           &dom,
                                 custodian              &cst,
                                 writable<double>       &C)
        {
            YACK_XMLSUB(xml,"collector::balance");


            unsigned count   = 0;
            cst.process(xml,C,dom);

            //------------------------------------------------------------------
            //
            // start loop
            //
            //------------------------------------------------------------------
        LOOP:
            bool     changed = false;
            ++count;
            if(xml.verbose)
            {
                *xml << "/----------------" << std::endl;
                *xml << "| #loop = " << std::setw(4) << count << std::endl;
                *xml << "\\----------------" << std::endl;
                for(const sp_node *sn=dom.live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const double   c  = C[sp.indx[top_level]];
                    dom.spfmt.pad(*xml << sp, sp) << " = " << std::setw(15) << c << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            // sorting out equilibria
            //
            //------------------------------------------------------------------
            initialize();
            collector_ &self = *this;
            for(const eq_node *en = dom.defined.head; en; en=en->next)
            {
                const equilibrium    &eq  = ***en;
                const size_t          ei  = eq.indx[cat_level];
                chart                &ch  = self[ei];

                //--------------------------------------------------------------
                //
                // get status according to STRICT NEGATIVITY
                //
                //--------------------------------------------------------------
                const chart::oor_type oor = ch.settle(eq,C,dom.reg);
                switch(oor)
                {
                    case chart::oor_none:
                        balanced << eq;
                        continue;

                    case chart::oor_both:
                        if(xml.verbose)  dom.eqfmt.pad(*xml << "blocked   : " << eq,eq) << " : "  << ch << std::endl;
                        singular << eq;
                        continue;

                    case chart::oor_prod:
                        if(xml.verbose)  dom.eqfmt.pad(*xml << "prod(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        if(!dispatch(eq,ch.adjust_prod(xml))) continue;
                        break;

                    case chart::oor_reac:
                        if(xml.verbose)  dom.eqfmt.pad(*xml << "reac(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        if(!dispatch(eq,ch.adjust_reac(xml))) continue;
                        break;

                }
                assert(chart::oor_prod == oor || chart::oor_reac == oor );
                assert(solvable.contains(eq) || weakened.contains(eq) );

                //--------------------------------------------------------------
                //
                // create a more balanced phase space
                //
                //--------------------------------------------------------------
                gain[ei] = compute_balanced(xadd,Cbal[ei],eq,C,dom.reg,ch.corr);

                if(xml.verbose)
                {
                    *xml <<" |_extent : " << ch.corr << std::endl;
                    eq.display_compact(*xml <<" |_origin : ",C)        << std::endl;
                    eq.display_compact(*xml <<" |_target : ",Cbal[ei]) << std::endl;
                    *xml <<" |_gain   : " << gain[ei] << std::endl;
                }

            }

            assert(balanced.size+solvable.size+weakened.size+singular.size==dom.defined.size);
            if(xml.verbose)
            {
                if(balanced.size) *xml << "|_balanced=" << balanced << std::endl;
                if(solvable.size) *xml << "|_solvable=" << solvable << std::endl;
                if(weakened.size) *xml << "|_weakened=" << weakened << std::endl;
                if(singular.size) *xml << "|_singular=" << singular << std::endl;
            }



            //------------------------------------------------------------------
            //
            // check solvable
            //
            //------------------------------------------------------------------
            if(solvable.size)
            {
                YACK_XMLOG(xml, "-------- solvable -------- #" << solvable.size);
                if(xml.verbose) display_gains(xml,gain,dom.eqfmt,solvable.head);
                displace(C,find_opt(dom.retaking, solvable, xml));
                changed = true;
                goto LOOP;
            }

            //------------------------------------------------------------------
            //
            //  check weakened
            //
            //------------------------------------------------------------------
            if(weakened.size)
            {
                YACK_XMLOG(xml, "-------- weakened -------- #" << weakened.size);
                if(xml.verbose) display_gains(xml,gain,dom.eqfmt,weakened.head);
                displace(C,find_opt(dom.retaking, weakened, xml));
                changed = true;
                goto LOOP;
            }

            //------------------------------------------------------------------
            //
            //  and adjust splitting/combining
            //
            //------------------------------------------------------------------
            if(dom.splitting.size)
            {
                YACK_XMLOG(xml, "-------- splitting ------- #" << dom.splitting.size);

                for(const eq_node *en=dom.splitting.head;en;en=en->next)
                {
                    const equilibrium     &eq  = ***en;
                    assert(eq.reac.size<=0);
                    assert(eq.prod.size>0);
                    if(needed_some_fixing(eq.prod,C))
                    {
                        changed=true;
                        if(xml.verbose) eq.display_compact( dom.eqfmt.pad(*xml << eq,eq) << " : ",C) << std::endl;
                    }
                }
            }

            if(dom.combining.size)
            {
                YACK_XMLOG(xml, "-------- combining ------- #" << dom.combining.size);
                for(const eq_node *en=dom.combining.head;en;en=en->next)
                {
                    const equilibrium     &eq  = ***en;
                    assert(eq.reac.size>0);
                    assert(eq.prod.size<=0);
                    if(needed_some_fixing(eq.reac,C))
                    {
                        changed=true;
                        if(xml.verbose) eq.display_compact( dom.eqfmt.pad(*xml << eq,eq) << " : ",C) << std::endl;
                    }
                }
            }


            if(singular.size)
            {
                YACK_XMLOG(xml, "-------- singular -------- #" << singular.size);
                exit(0);
                cst.process(xml,C,dom);
                goto LOOP;
            }



            if(changed&&xml.verbose)
            {
                *xml << "/----------------\\" << std::endl;
                *xml << "|    balanced    |" << std::endl;
                *xml << "\\----------------/" << std::endl;
                for(const sp_node *sn=dom.live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const double   c  = C[sp.indx[top_level]];
                    dom.spfmt.pad(*xml << sp, sp) << " = " << std::setw(15) << c << std::endl;
                }
            }




        }

        void collector:: displace(writable<double> &C,
                                  const cluster    &W)
        {
            if(1==W.size)
            {
            }


            for(const sp_node *sn=W.spec.head;sn;sn=sn->next)
            {
                const species &s = ***sn;
                const size_t   j = s.indx[top_level];
                const double   c = C[j];
                xadd.free();
                xadd.push(c);
                for(const eq_node *en=W.head;en;en=en->next)
                {
                    const equilibrium      &E  = ***en;
                    const size_t            i  = E.indx[cat_level];
                    xadd.push(-c);
                    xadd.push(Cbal[i][j]);
                }
                C[j] = xadd.sum();
            }

            //std::cerr << "C=" << C << std::endl;

        }

        bool   collector:: needed_some_fixing(const actors &A, writable<double> &C)
        {
            assert(A.size>0);
            solo.clear();
            for(const actor *a=A.head;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[top_level]];
                if(c<0)
                {
                    solo.insert( (-c)/a->nu, s);
                }
            }
            //std::cerr << "solo: " << solo << std::endl;
            if(solo.size>0)
            {
                const zlimit &zl = **(solo.tail);
                const double  xi = zl.extent;
                for(const actor *a=A.head;a;a=a->next)
                {
                    const species &s = **a;
                    C[ s.indx[top_level] ] += (a->nu * xi);
                }
                zl.nullify(C);
                return true;
            }
            else
            {
                return false;
            }


        }


    }
    
}


