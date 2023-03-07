
#include "yack/aqueous/species.hpp"

namespace yack
{
    namespace aqueous
    {
        species:: ~species() noexcept
        {
        }


        std::ostream & operator<<(std::ostream &os, const species &self)
        {
            os << '[' << self.name << ']';
            return os;
        }

    }

}

