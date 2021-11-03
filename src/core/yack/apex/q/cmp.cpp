#include "yack/apex/rational.hpp"

namespace yack
{

    namespace apex
    {

        int rational:: compare(const rational &lhs, const rational &rhs)
        {
            const  integer L = lhs.num * rhs.den;
            const  integer R = rhs.num * lhs.den;
            return integer::compare(L,R);
        }

        int compare(const rational &lhs, const integer &rhs)
        {
            const  integer R = lhs.den * rhs;
            return integer::compare(lhs.num,R);
        }

        int compare(const rational &lhs, const int_type  rhs)
        {
            const  integer R = lhs.den * rhs;
            return integer::compare(lhs.num,R);
        }
        
        
        int compare(const integer  &lhs, const rational &rhs)
        {
            const integer L = lhs * rhs.den;
            return integer::compare(lhs,rhs.num);
        }
        
        int compare(const int_type  lhs, const rational &rhs)
        {
            const integer L = lhs * rhs.den;
            return integer::compare(lhs,rhs.num);
        }

    }

}
