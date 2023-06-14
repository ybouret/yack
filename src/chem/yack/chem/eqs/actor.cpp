#include "yack/chem/eqs/actor.hpp"

namespace yack
{
    namespace Chemical
    {
        Actor:: ~Actor() noexcept
        {

        }

        Actor:: Actor(const unsigned n, const Species &s) noexcept :
        object(),
        nu(n),
        xn(nu),
        nm(nu-1),
        sp(s),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &other) noexcept :
        object(),
        nu( other.nu ),
        xn( other.xn ),
        nm( other.nm ),
        sp( other.sp ),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        const Species & Actor:: operator*() const noexcept
        {
            return sp;
        }


        void Actor:: displayFirst(std::ostream &os) const
        {
            assert(nu>0);
            if(nu>1) os << nu << '*';
            os << sp;
        }



        void Actor:: displayExtra(std::ostream &os) const
        {
            assert(nu>0);
            os << '+';
            displayFirst(os);
        }


        string Actor:: displayFirst() const
        {
            string res;
            if(nu>1) res += vformat("%u*",nu);
            res += '[' + sp.name + ']';
            return res;
        }


        string Actor:: displayExtra() const
        {
            assert(nu>0);
            return '+' + displayFirst();
        }

        void Actor:: massAction(Extended::Mul                  &xmul,
                                const readable<Extended::Real> &C,
                                const IndexLevel                level) const
        {
            assert(nu>0);
            xmul.insert( C[ sp.indx[level] ], nu);
        }


    }

}


