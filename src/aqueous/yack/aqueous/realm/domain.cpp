
#include "yack/aqueous/realm/domain.hpp"

namespace yack
{
    namespace aqueous
    {
        domain:: ~domain() noexcept
        {
        }

        domain:: domain(const equilibrium &first)   :
        large_object(),
        eq_list(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        bool domain:: accepts(const equilibrium &rhs) const noexcept
        {
            for(const eq_node *en=head;en;en=en->next)
            {
                const equilibrium &lhs = ***en; assert(&lhs!=&rhs);
                if(lhs.is_connected_to(rhs)) return true;
            }
            return false;
        }

        bool domain:: accepts(const domain &rhs) const noexcept
        {
            // loop over rhs eqs
            for(const eq_node *en=rhs.head;en;en=en->next)
            {
                if(accepts(***en)) return true;
            }
            return false;
        }


    }

}
