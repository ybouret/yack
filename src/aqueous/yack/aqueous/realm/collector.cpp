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
        collector_(n,*this)
        {
        }

        void collector:: probe(const eq_list          &eqs,
                               const readable<double> &C,
                               const readable<bool>   &R)
        {
            collector_ &self = *this;
            for(const eq_node *en = eqs.head; en; en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[cat_level];
                self[ei].settle(eq,C,R);

                std::cerr << eq << " : "  << self[ei] << std::endl;
            }
        }

    }

}


