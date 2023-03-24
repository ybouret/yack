#include "yack/aqueous/realm/collector.hpp"
#include "yack/math/iota.hpp"

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
        Gain(n,0),
        Cbal(n,m),
        xadd()
        {
        }

        static inline
        double compute_balanced(cameo::add<double>    &xadd,
                              writable<double>       &Cb,
                              const components       &eq,
                              const readable<double> &C0,
                              const readable<bool>   &R,
                              const zlimit           &zl) noexcept
        {

            //------------------------------------------------------------------
            //
            // load all phase space
            //
            //------------------------------------------------------------------
            math::iota::load(Cb,C0);

            //------------------------------------------------------------------
            //
            // apply extent
            //
            //------------------------------------------------------------------
            const double xi = zl.extent;
            for(const cnode *node=eq->head;node;node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const size_t     j  = sp.indx[top_level];
                Cb[j] += xi * cc.nu;
            }

            //------------------------------------------------------------------
            //
            // use vanishing
            //
            //------------------------------------------------------------------
            for(const sp_node *zn=zl.head;zn;zn=zn->next)
            {
                Cb[ (***zn).indx[top_level] ] = 0;
            }

            //------------------------------------------------------------------
            //
            // deduce gain
            //
            //------------------------------------------------------------------
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

        void collector:: initialize() noexcept
        {
            balanced.clear();
            solvable.clear();
            weakened.clear();
            singular.clear();
        }

        static inline void display_gains(const xmlog            &xml,
                                         const readable<double> &Gain,
                                         const gathering        &fmt,
                                         const eq_node          *en)
        {

            for(;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[cat_level];
                fmt.pad(*xml << eq, eq) << " : " << Gain[ei] << std::endl;
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
                const double gain = Gain[ei] = compute_balanced(xadd,Cbal[ei],eq,C,R,ch.corr);
                if(xml.verbose)
                {
                    *xml <<" |_extent : " << ch.corr << std::endl;
                    eq.display_compact(*xml <<" |_origin : ",C)        << std::endl;
                    eq.display_compact(*xml <<" |_target : ",Cbal[ei]) << std::endl;
                    *xml <<" |_gain   : " << gain << std::endl;
                }
            }

            YACK_XMLOG(xml, "balanced : " << balanced);
            YACK_XMLOG(xml, "solvable : " << solvable);
            YACK_XMLOG(xml, "weakened : " << weakened);
            YACK_XMLOG(xml, "singular : " << singular);

            if(solvable.size)
            {
                YACK_XMLOG(xml, "-------- solvable -------- #" << solvable.size);
                if(xml.verbose) display_gains(xml,Gain,fmt,solvable.head);
                for(const cluster *cls=retaking.head;cls;cls=cls->next)
                {
                    if(cls->is_subset_of(solvable))
                    {
                        std::cerr << "--> should try " << *cls << std::endl;
                    }
                }
            }


            if(weakened.size)
            {
                YACK_XMLOG(xml, "-------- weakened -------- #" << weakened.size);
                if(xml.verbose) display_gains(xml,Gain,fmt,weakened.head);
                for(const cluster *cls=retaking.head;cls;cls=cls->next)
                {
                    if(cls->is_subset_of(weakened))
                    {
                        std::cerr << "--> should try " << *cls << std::endl;
                    }
                }
            }





        }

    }

}


