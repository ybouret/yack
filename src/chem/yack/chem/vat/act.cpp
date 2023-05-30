#include "yack/chem/vat/act.hpp"

namespace yack
{
    namespace Chemical
    {

        Act:: Act(const ConservationLaw &first) : LawRepo(), next(0), prev(0)
        {
            *this << first;
        }

        Act:: ~Act() noexcept
        {
        }

        bool Act:: endorses(const ConservationLaw &law) const noexcept
        {
            for(const LawNode *node=head;node;node=node->next)
            {
                const ConservationLaw &lhs = ***node;
                if(lhs.linkedTo(law))
                    return true;
            }
            return false;
        }

        bool Act:: linkedTo(const Act &other) const noexcept
        {
            for(const LawNode *node=other.head;node;node=node->next)
            {
                const ConservationLaw &law = ***node;
                if(endorses(law)) return true;
            }
            return false;
        }

        void Act:: finalize() noexcept
        {
            for(const LawNode *node=head;node;node=node->next)
            {
                const ConservationLaw &law = ***node;
                update(law);
            }
        }



    }

}
