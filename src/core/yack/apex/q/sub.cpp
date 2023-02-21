

#include "yack/apex/rational.hpp"

namespace yack
{

    namespace apex
    {
        rational rational:: sub(const rational &lhs, const rational &rhs)
        {
            const  natural den = lhs.den * rhs.den;
            const  integer L   = lhs.num * rhs.den;
            const  integer R   = rhs.num * lhs.den;
            const  integer num = L-R;
            return rational(num,den);
        }


#define YACK_APQ_SUB_RHS()               \
const integer::handle lh(lhs.den);       \
const integer::handle rh(rhs);           \
const integer R   = integer::mul(lh,rh); \
const integer num = lhs.num-R;           \
return rational(num,lhs.den);

        rational rational:: sub(const rational &lhs, const integer &rhs)
        {
            YACK_APQ_SUB_RHS();
        }

        rational rational:: sub(const rational &lhs, const int_type  rhs)
        {
            YACK_APQ_SUB_RHS();
        }

        rational rational:: sub(const rational &lhs, const natural  &rhs)
        {
            YACK_APQ_SUB_RHS();
        }

#define YACK_APQ_SUB_LHS()                     \
const integer::handle lh(lhs);                 \
const integer::handle rh(rhs.den);             \
const integer         L = integer::mul(lh,rh); \
const integer         num = L - rhs.num;       \
return rational(num,rhs.den);


        rational rational:: sub(const integer  &lhs, const rational &rhs)
        {
            YACK_APQ_SUB_LHS();
        }

        rational rational:: sub(const int_type  lhs, const rational &rhs)
        {
            YACK_APQ_SUB_LHS();
        }

        rational rational:: sub(const natural  &lhs, const rational &rhs)
        {
            YACK_APQ_SUB_LHS();
        }

        void rational:: neg() noexcept
        {
            coerce(num).neg();
        }


        rational  rational:: operator-() const
        {
            rational q(*this);
            __sign::change( coerce(q.num.s) );
            return q;
        }


        rational & rational:: operator--()
        {
            const int_type one = 1;
            rational       I   = sub(*this,one);
            xch(I);
            return *this;
        }

        rational rational:: operator--(int)
        {
            const int_type one = 1;
            rational       I   = sub(*this,one);
            xch(I);
            return I;
        }

    }

}

