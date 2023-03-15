
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
        N(0),
        M(0),
        last(NULL),
        live(),
        roaming(),
        defined(),
        endless(),
        bounded(),
        Nu(),
        NuT(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        int domain:: compare(const eq_node *lhs, const eq_node *rhs) noexcept
        {
            const equilibrium &L = ***lhs;
            const equilibrium &R = ***rhs;
            return comparison::increasing(L.indx[0],R.indx[0]);
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
