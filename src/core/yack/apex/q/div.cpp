#include "yack/apex/rational.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{

    namespace apex
    {

        static inline rational make_div(integer       &num,
                                        const integer &den)
        {
            switch( __sign::product(num.s,den.s) )
            {
                case positive: assert(num.s!=__zero__); coerce(num.s) = positive; return rational(num,den.n);
                case negative: assert(num.s!=__zero__); coerce(num.s) = negative; return rational(num,den.n);
                default:
                    break;
            }
            assert(__zero__ == __sign::product(num.s,den.s) );
            assert(0 == num);
            return rational();
        }

        rational rational:: div(const rational &lhs, const rational &rhs)
        {
            if(rhs.num == 0) throw libc::exception(EDOM,"%s: division by zero %s",clid,clid);
            integer       num = lhs.num * rhs.den;
            const integer den = lhs.den * rhs.num;
            return make_div(num,den);
        }

        // div by RHS

        rational rational:: div(const rational &lhs, const integer &rhs)
        {
            if(rhs == 0) throw libc::exception(EDOM,"%s division by zero %s",clid,integer::clid);

            const natural   den = lhs.den * rhs.n;
            const sign_type sgn = __sign::product(lhs.num.s,rhs.s);
            const integer   num(sgn,lhs.num.n);
            return rational(num,den);
        }

        rational rational:: div(const rational &lhs, const natural &rhs)
        {
            if(rhs == 0) throw libc::exception(EDOM,"%s division by zero %s",clid,natural::clid);

            const natural   den = lhs.den * rhs;
            return rational(lhs.num,den);
        }

        rational rational:: div(const rational &lhs, const int_type rhs)
        {
            if(rhs == 0) throw libc::exception(EDOM,"%s division by zero %s",clid,"int");

            const integer::handle rh(rhs);
            const integer::handle lh(lhs.den);
            const integer         den = integer::mul(lh,rh);
            integer               num = lhs.num;
            return make_div(num,den);
        }

        // LHS divided by rational
        rational rational:: div(const integer &lhs, const rational &rhs)
        {
            if(rhs.num==0) throw exception("%s: division by zero %s",integer::clid,clid);
            integer num = lhs * rhs.den;
            return make_div(num,rhs.num);
        }

        rational rational:: div(const natural &lhs, const rational &rhs)
        {
            if(rhs.num==0) throw exception("%s: division by zero %s",natural::clid,clid);
            const natural nn = lhs*rhs.den;
            const integer num(rhs.num.s,nn);
            return rational(num,rhs.num.n);
        }


        rational rational:: div(const int_type lhs, const rational &rhs)
        {
            if(rhs.num==0) throw exception("%s: division by zero %s",integer::clid,clid);
            const integer::handle lh(lhs);
            const integer::handle rh(rhs.den);
            integer num = integer::mul(lh,rh);
            return make_div(num,rhs.num);
        }
    }

}
