
#include "yack/apex/rational.hpp"

namespace yack
{

    namespace apex
    {
        rational rational:: add(const rational &lhs, const rational &rhs)
        {
            const natural den = lhs.den * rhs.den;
            const integer L   = lhs.num * rhs.den;
            const integer R   = rhs.num * lhs.den;
            const integer num = L+R;
            return rational(num,den);
        }

    }

}

