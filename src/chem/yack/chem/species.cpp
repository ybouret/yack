#include "yack/chem/species.hpp"

namespace yack
{
    namespace Chemical
    {

        bool Species::Verbose = false;
        
        Species:: ~Species() noexcept
        {
            
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << '[' << sp.name << ']';
            return os;
        }

    }

}
