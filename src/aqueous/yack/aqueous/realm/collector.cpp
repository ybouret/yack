#include "yack/aqueous/realm/collector.hpp"

namespace yack
{
    namespace aqueous
    {

        
        collector:: ~collector() noexcept
        {

        }

        collector:: collector(const size_t n) :
        collecting::caches(),
        collector_(n,*this),
        unbal()
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
            collector_ &self = *this;
            for(const eq_node *en = eqs.head; en; en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[cat_level];
                chart             &ch = self[ei];
                const bool         ok = ch.settle(eq,C,R);
                if(xml.verbose) fmt.pad(*xml << eq,eq) << " : "  << ch << std::endl;
                if(!ok) unbal << eq;
            }
            YACK_XMLOG(xml, "unbalanced = " << unbal);

        }

    }

}


