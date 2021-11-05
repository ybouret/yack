
#include "yack/apex/rational.hpp"

namespace yack
{

    namespace apex
    {
        rational rational:: add(const rational &lhs, const rational &rhs)
        {
            const  natural den = lhs.den * rhs.den;
            const  integer L   = lhs.num * rhs.den;
            const  integer R   = rhs.num * lhs.den;
            const  integer num = L+R;
            return rational(num,den);
        }

#define YACK_APQ_ADD_RHS()         \
const integer R   = rhs * lhs.den; \
const integer num = lhs.num + R;   \
return rational(num,lhs.den)

        rational rational:: add(const rational &lhs, const integer &rhs)
        {
            YACK_APQ_ADD_RHS();
        }

        rational rational:: add(const rational &lhs, const int_type  rhs)
        {
            YACK_APQ_ADD_RHS();
        }

        rational rational:: add(const rational &lhs, const natural  &rhs)
        {
            YACK_APQ_ADD_RHS();
        }

#define YACK_APQ_ADD_LHS()         \
const integer L   = lhs * rhs.den; \
const integer num = L + rhs.num;   \
return rational(num,rhs.den)

        rational rational:: add(const integer  &lhs, const rational &rhs)
        {
            YACK_APQ_ADD_LHS();
        }

        rational rational:: add(const int_type  lhs, const rational &rhs)
        {
            YACK_APQ_ADD_LHS();
        }

        rational rational:: add(const natural  &lhs, const rational &rhs)
        {
            YACK_APQ_ADD_LHS();
        }

        rational  rational:: operator+() const
        {
            return *this;
        }


        rational & rational:: operator++()
        {
            const int_type one = 1;
            rational       I   = add(*this,one);
            xch(I);
            return *this;
        }

        rational rational:: operator++(int)
        {
            const int_type one = 1;
            rational       I   = add(*this,one);
            xch(I);
            return I;
        }

    }

}

