#include "yack/chem/species.hpp"

namespace yack
{
    namespace Chemical
    {

        Species:: ~Species() noexcept
        {
            
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << '[' << sp.name << ']' << std::endl;
            return os;
        }

    }

}
