#include "yack/aqueous/realm/collector.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
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
        gain(n,0),
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
                for(const sp_node *zn=zl.head;zn;zn=zn->next)
                {
                    Cb[ (***zn).indx[top_level] ] = 0;
                }

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

            static inline
            double  gain_of(const cluster          &clan,
                            const readable<double> &gain,
                            cameo::add<double>     &xadd)
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

            static inline
            const cluster *find_best(const partition        &part,
                                     const eq_repo          &zeqs,
                                     const readable<double> &gain,
                                     cameo::add<double>     &xadd,
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
                opt = gain_of(*win,gain,xadd);

                YACK_XMLOG(xml, "(+) " << std::setw(15) << opt << " @" << *win);
                for(const cluster *cls=win->next;cls;cls=cls->next)
                {
                    if(!cls->is_subset_of(zeqs)) continue;
                    const double tmp = gain_of(*cls,gain,xadd);
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
                return win;
            }
        }


        void collector:: probe(const xmlog            &xml,
                               const gathering        &fmt,
                               const eq_list          &eqs,
                               const readable<double> &C,
                               const readable<bool>   &R,
                               const partition        &retaking)
        {
            YACK_XMLSUB(xml,"collector::probe");
            YACK_XMLOG(xml, "detecting unbalanced");

            //------------------------------------------------------------------
            //
            // initializing
            //
            //------------------------------------------------------------------
            initialize();

            //------------------------------------------------------------------
            //
            // first loop: classify
            //
            //------------------------------------------------------------------
            collector_ &self = *this;
            for(const eq_node *en = eqs.head; en; en=en->next)
            {
                const equilibrium    &eq  = ***en;
                const size_t          ei  = eq.indx[cat_level];
                chart                &ch  = self[ei];
                const chart::oor_type oor = ch.settle(eq,C,R);
                switch(oor)
                {
                    case chart::oor_none:
                        if(xml.verbose)  fmt.pad(*xml << "balanced  : " << eq,eq) << std::endl;
                        balanced << eq;
                        continue;

                    case chart::oor_both:
                        if(xml.verbose)  fmt.pad(*xml << "blocked   : " << eq,eq) << " : "  << ch << std::endl;
                        singular << eq;
                        continue;

                    case chart::oor_prod:
                        if(xml.verbose)  fmt.pad(*xml << "prod(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        if(ch.adjust_prod(xml))
                        {
                            solvable << eq;
                        }
                        else
                        {
                            weakened << eq;
                        }
                        break;

                    case chart::oor_reac:
                        if(xml.verbose)  fmt.pad(*xml << "reac(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        if(ch.adjust_reac(xml))
                        {
                            solvable << eq;
                        }
                        else
                        {
                            weakened << eq;
                        }
                        break;
                }

                assert(chart::oor_prod == oor || chart::oor_reac == oor );
                assert(solvable.contains(eq) || weakened.contains(eq) );
                gain[ei] = compute_balanced(xadd,Cbal[ei],eq,C,R,ch.corr);
                if(xml.verbose)
                {
                    *xml <<" |_extent : " << ch.corr << std::endl;
                    eq.display_compact(*xml <<" |_origin : ",C)        << std::endl;
                    eq.display_compact(*xml <<" |_target : ",Cbal[ei]) << std::endl;
                    *xml <<" |_gain   : " << gain[ei] << std::endl;
                }
            }

            YACK_XMLOG(xml, "balanced : " << balanced);
            YACK_XMLOG(xml, "solvable : " << solvable);
            YACK_XMLOG(xml, "weakened : " << weakened);
            YACK_XMLOG(xml, "singular : " << singular);

            if(solvable.size)
            {
                YACK_XMLOG(xml, "-------- solvable -------- #" << solvable.size);
                if(xml.verbose) display_gains(xml,gain,fmt,solvable.head);
                const cluster *win = find_best(retaking, solvable, gain, xadd, xml);
            }


            if(weakened.size)
            {
                YACK_XMLOG(xml, "-------- weakened -------- #" << weakened.size);
                if(xml.verbose) display_gains(xml,gain,fmt,weakened.head);
                const cluster *win = find_best(retaking, weakened, gain, xadd, xml);

            }





        }

    }
    
}


