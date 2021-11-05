
#include "yack/apex/rational.hpp"

namespace yack
{

    namespace apex
    {
        rational rational:: mul(const rational &lhs, const rational &rhs)
        {
            const  integer num = lhs.num * rhs.num;
            const  natural den = lhs.den * rhs.den;
            return rational(num,den);
        }

#define YACK_APQ_MUL_RHS()                       \
const integer::handle lh(lhs.num);               \
const integer::handle rh(rhs);                   \
const integer         num = integer::mul(lh,rh); \
return rational(num,lhs.den);

        rational rational:: mul(const rational &lhs, const integer &rhs)
        {
            YACK_APQ_MUL_RHS();
        }

        rational rational:: mul(const rational &lhs, const int_type  rhs)
        {
            YACK_APQ_MUL_RHS();
        }

        rational rational:: mul(const rational &lhs, const natural  &rhs)
        {
            YACK_APQ_MUL_RHS();
        }


#define YACK_APQ_MUL_LHS()                       \
const integer::handle lh(lhs);                   \
const integer::handle rh(rhs.num);               \
const integer         num = integer::mul(lh,rh); \
return rational(num,rhs.den);

        rational rational:: mul(const integer  &lhs, const rational &rhs)
        {
            YACK_APQ_MUL_LHS();
        }

        rational rational:: mul(const int_type  lhs, const rational &rhs)
        {
            YACK_APQ_MUL_LHS();
        }

        rational rational:: mul(const natural  &lhs, const rational &rhs)
        {
            YACK_APQ_MUL_LHS();
        }



    }

}

