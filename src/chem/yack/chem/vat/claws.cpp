
#include "yack/chem/vat/claws.hpp"

namespace yack
{
    namespace Chemical
    {
        ConservationLaws:: ConservationLaws() noexcept
        {
        }

        ConservationLaws:: ~ConservationLaws() noexcept
        {
        }

        void ConservationLaws:: finalize()
        {
            coerce(maxlen) = 0;
            for(ConservationLaw *claw=head;claw;claw=claw->next)
            {
                claw->makeName();
                claw->finalize();
                update(*claw);
            }
        }

        std::ostream &  operator<<(std::ostream &os, const ConservationLaws &laws)
        {

            os << '{' << std::endl;
            for(const ConservationLaw *claw=laws.head;claw;claw=claw->next)
            {
                laws.pad(os << '\t' << *claw, *claw) << " = 0" << std::endl;
            }
            os << '}';
            return os;
        }

    }

}

