#include "yack/apex/integer.hpp"

namespace yack
{
    namespace apex
    {
        integer integer:: operator-() const
        {
            return integer( __sign::opposite(s), n );
        }

        integer integer::sub(const handle &lh, const handle &rh)
        {
            __sign::change( coerce(rh.s) );
            return add(lh,rh);
        }

        integer & integer:: operator--()
        {
            static const handle R(-1);
            const handle        L(*this);
            integer             I = add(L,R);
            xch(I);
            return *this;
        }

        integer integer:: operator--(int)
        {
            static const handle R(-1);
            const handle        L(*this);
            integer             I = add(L,R);
            xch(I);
            return I;
        }


    }
}
