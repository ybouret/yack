
#include "yack/chem/eqs/const.hpp"

namespace yack
{
    namespace chemical
    {
        const_equilibrium:: ~const_equilibrium() throw()
        {
        }

        double const_equilibrium:: getK(const double) const
        {
            return value;
        }

    }

}

