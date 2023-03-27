
#include "yack/aqueous/realm/collector.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace aqueous
    {
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





        bool collector:: dispatch(const equilibrium &eq,
                                  const shift_status st)
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
                                 writable<double>       &C)
        {
            YACK_XMLSUB(xml,"collector::balance");


            unsigned count   = 0;
            bool     changed = false;
            //------------------------------------------------------------------
            //
            // start loop
            //
            //------------------------------------------------------------------
        LOOP:
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
            for(const eq_node *en = dom.defined.head; en; en=en->next)
            {
                const equilibrium    &eq  = ***en;
                const size_t          ei  = eq.indx[cat_level];
                chart                &ch  = charts[ei];

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
                if(balanced.size) *xml << "|_balanced = " << balanced << std::endl;
                if(solvable.size) *xml << "|_solvable = " << solvable << std::endl;
                if(weakened.size) *xml << "|_weakened = " << weakened << std::endl;
                if(singular.size) *xml << "|_singular = " << singular << std::endl;
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
                changed = true;
                YACK_XMLOG(xml, "-------- singular -------- #" << singular.size);
                for(const sp_node *sn = dom.live.head;sn;sn=sn->next)
                {
                    const species &s = ***sn;
                    const size_t   j = s.indx[top_level];
                    double        &c = C[j];
                    if(c<0)
                    {
                        (*this)[j] += (-c);
                        c = 0;
                    }
                }
            }



            if(changed&&xml.verbose)
            {
                *xml << "/----------------\\" << std::endl;
                *xml << "|    balanced    |" << std::endl;
                *xml << "\\----------------/" << std::endl;
                for(const sp_node *sn=dom.live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const size_t   j  = sp.indx[top_level];
                    const double   c  = C[j];
                    dom.spfmt.pad(*xml << sp, sp) << " = " << std::setw(15) << c << " + " << (*this)[j] << std::endl;
                }
            }



        }


    }

}


