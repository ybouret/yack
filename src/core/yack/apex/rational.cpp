
#include "yack/apex/rational.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>


namespace yack
{

    namespace apex
    {

        const char rational:: clid[] = "apq";


        rational:: ~rational() throw() {}


        rational:: rational() : number(), num(0), den(1) {}

        void rational:: xch(rational &q) throw()
        {
            coerce(num).xch( coerce(q.num) );
            coerce(den).xch( coerce(q.den) );
        }

        rational rational:: abs() const
        {
            rational tmp  = *this;
            if(tmp.num.s==negative)
            {
                coerce(tmp.num.s) = positive;
            }
            return tmp;
        }

        rational rational:: mod2() const
        {
            const  integer num2 = num.mod2();
            const  natural den2 = den.mod2();
            return rational(num2,den2);
        }



        rational:: rational(const int_type N) : number(), num(N), den(1) {}
        

        rational:: rational(const integer &N) : number(), num(N), den(1) {}

        rational:: rational(const rational &q) :
        number(),
        num(q.num),
        den(q.den)
        {
        }

        rational:: rational(const sign_type S, const natural &N) : number(), num(S,N), den(1) {}


        rational:: rational(const natural &N) : num(N), den(1) {}
        

        rational& rational:: operator=(const rational &q)
        {
            rational tmp(q);
            xch(tmp);
            return *this;
        }



        void rational:: zcheck() const
        {
            if(0==den) throw libc::exception(EDOM,"rational(zero denominator)");
        }

        rational:: rational(randomized::bits &ran, const size_t nbits, const size_t dbits) :
        num(ran,nbits),
        den(ran,dbits<=0?1:dbits)
        {
            reduce();
        }
        

        std::ostream &operator<<(std::ostream &os, const rational &q)
        {
            if(q.den==1)
            {
                os << q.num;
            }
            else
            {
                os << '(' << q.num << '/' << q.den << ')';
            }
            return os;
        }

        void rational:: reduce()
        {
            natural::simplify(coerce(num.n),coerce(den));
        }

        void rational:: setup()
        {
            zcheck();
            reduce();
        }


        const char * rational:: class_uid() const throw() { return clid; }
        size_t       rational:: serialize(ios::ostream &fp) const
        {
            size_t nw = num.serialize(fp);
            return nw+den.serialize(fp);
        }

    }

}

