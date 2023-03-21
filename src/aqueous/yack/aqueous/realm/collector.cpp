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
        unbal(),
        Cbal(n,m),
        good(n,false)
        {
        }

        static inline
        void compute_balanced(writable<double>       &Cb,
                              const components       &eq,
                              const readable<double> &C0,
                              const zlimit           &zl) noexcept
        {
            math::iota::load(Cb,C0);
            const double xi = zl.extent;
            for(const cnode *node=eq->head;node;node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const size_t     j  = sp.indx[top_level];
                Cb[j] += xi * cc.nu;
            }
            for(const sp_node *zn=zl.head;zn;zn=zn->next)
            {
                Cb[ (***zn).indx[top_level] ] = 0;
            }

        }

        void collector:: probe(const xmlog            &xml,
                               const gathering        &fmt,
                               const eq_list          &eqs,
                               const readable<double> &C,
                               const readable<bool>   &R)
        {
            YACK_XMLSUB(xml,"collector::probe");
            YACK_XMLOG(xml, "detecting unbalanced");

            unbal.clear();
            good.ld(false);

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
                        continue;

                    case chart::oor_both:
                        if(xml.verbose)  fmt.pad(*xml << "blocked   : " << eq,eq) << " : "  << ch << std::endl;
                        continue;

                    case chart::oor_prod:
                        if(xml.verbose)  fmt.pad(*xml << "prod(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        good[ei] = ch.adjust_prod();
                        break;

                    case chart::oor_reac:
                        if(xml.verbose)  fmt.pad(*xml << "reac(s)<0 : " << eq,eq) << " : "  << ch << std::endl;
                        good[ei] = ch.adjust_reac();
                        break;

                }
                
                assert(chart::oor_prod == oor || chart::oor_reac == oor );
                unbal << eq;
                compute_balanced(Cbal[ei],eq,C,ch.corr);
            }

        }

    }

}


