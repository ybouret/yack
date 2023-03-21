#include "yack/aqueous/realm/collector.hpp"

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
                const equilibrium &eq  = ***en;
                const size_t       ei  = eq.indx[cat_level];
                chart             &ch  = self[ei];
                switch(ch.settle(eq,C,R))
                {
                    case chart::oor_none:
                        break;

                    case chart::oor_both:
                        if(xml.verbose)  fmt.pad(*xml << "blocked   " << eq,eq) << " : "  << ch << std::endl;
                        break;

                    case chart::oor_prod:
                        if(xml.verbose)  fmt.pad(*xml << "prod(s)<0 " << eq,eq) << " : "  << ch << std::endl;
                        break;

                    case chart::oor_reac:
                        if(xml.verbose)  fmt.pad(*xml << "reac(s)<0 " << eq,eq) << " : "  << ch << std::endl;
                        break;

                }
                
                if(chart::oor_none != ch.settle(eq, C, R))
                {

                }
                
                

            }

        }

    }

}


