
#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace Chemical
    {
        Equilibrium:: ~Equilibrium() noexcept {}

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            os << '<' << eq.name << '>';
            return os;
        }
    }

}


