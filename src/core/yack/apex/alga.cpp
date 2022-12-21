
#include "yack/apex/alga.hpp"

namespace yack
{

    template <>
    apq alga::ratio<apq>::of(const apq &lhs, const apq &rhs)
    {
        return lhs/rhs;
    }

}
