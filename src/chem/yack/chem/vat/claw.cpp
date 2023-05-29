
#include "yack/chem/vat/claw.hpp"

namespace yack
{
    namespace Chemical
    {
        ConservationLaw:: ConservationLaw()
        {
        }

        ConservationLaw:: ~ConservationLaw() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const ConservationLaw &claw)
        {
            const Actors &self = claw;
            os << "d_(";
            os << self;
            os << ")";
            return os;
        }

        bool ConservationLaw:: involves(const Species &sp) const noexcept
        {
            for(const Actor *a=head;a;a=a->next)
            {
                if( &sp == & **a) return true;
            }
            return false;
        }

        bool ConservationLaw:: linkedTo(const ConservationLaw &other) const noexcept
        {
            for(const Actor *a=other.head;a;a=a->next)
            {
                if(involves(**a)) return true;
            }
            return false;
        }
    }

}
