
#include "yack/chem/eqs/const.hpp"

namespace yack
{
    namespace Chemical
    {
        ConstEquilibrium:: ~ConstEquilibrium() noexcept {}

        Extended::Real ConstEquilibrium::getK(const double)
        {
            return K_;
        }

    }

}

