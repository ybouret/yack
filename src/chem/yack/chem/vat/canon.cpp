#include "yack/chem/vat/canon.hpp"

namespace yack
{
    namespace Chemical
    {

        Canon:: Canon(const ConservationLaw &first) : LawRepo(), next(0), prev(0)
        {
            *this << first;
        }

        Canon:: ~Canon() noexcept
        {
        }

        bool Canon:: endorses(const ConservationLaw &law) const noexcept
        {
            for(const LawNode *node=head;node;node=node->next)
            {
                const ConservationLaw &lhs = ***node;
                if(lhs.linkedTo(law))
                    return true;
            }
            return false;
        }

        bool Canon:: linkedTo(const Canon &other) const noexcept
        {
            for(const LawNode *node=other.head;node;node=node->next)
            {
                const ConservationLaw &law = ***node;
                if(endorses(law)) return true;
            }
            return false;
        }


    }

}
