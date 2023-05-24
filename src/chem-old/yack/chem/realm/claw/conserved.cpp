#include "yack/chem/realm/claw/conserved.hpp"

namespace yack
{
    namespace chemical
    {
        conserved:: ~conserved() noexcept
        {
        }

        conserved:: conserved(const conservation &first) :
        object(),
        conserved_(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        bool conserved:: accepts(const conservation &rhs) const noexcept
        {
            for(const node_type *node=head;node;node=node->next)
            {
                const conservation &lhs = ***node;
                if(lhs.overlaps(rhs)) return true;
            }
            return false;
        }

        bool conserved:: accepts(const conserved &rhs) const noexcept
        {
            for(const node_type *node=rhs.head;node;node=node->next)
            {
                const conservation &law = ***node;
                if(accepts(law)) return true;
            }
            return false;
        }
    }

}



