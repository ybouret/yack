
#include "yack/chem/vat/claw.hpp"

namespace yack
{
    namespace Chemical
    {
        ConservationLaw:: ConservationLaw() :
        Q2(),
        next(0),
        prev(0)
        {
        }

        ConservationLaw:: ~ConservationLaw() noexcept
        {
        }

        void ConservationLaw:: finalize()
        {
            Extended::Adder xadd;
            xadd.ensure(size);

            for(const Actor *a=head;a;a=a->next)
                xadd.append(a->xn);
            coerce(Q2) = xadd.reduce();
        }

        std::ostream & operator<<(std::ostream &os, const ConservationLaw &claw)
        {
            const Actors &self = claw;
            os << "d_(";
            os << self;
            os << ")";
            return os;
        }

        bool ConservationLaw:: involves(const Species &sp) const noexcept
        {
            for(const Actor *a=head;a;a=a->next)
            {
                if( &sp == & **a) return true;
            }
            return false;
        }

        bool ConservationLaw:: linkedTo(const ConservationLaw &other) const noexcept
        {
            for(const Actor *a=other.head;a;a=a->next)
            {
                if(involves(**a)) return true;
            }
            return false;
        }

        Extended::Real ConservationLaw:: excess(const readable<double> &C,
                                                Extended::Adder        &xadd,
                                                const IndexLevel        level) const
        {
            xadd.resume(size);
            const Actor          *ac = head;

            for(;ac;ac=ac->next )
            {
                Extended::Real p = Extended::Send( C[ (**ac).indx[level] ] );
                p *= ac->xn;
                xadd.append(p);
            }
            const Extended::Real xs = xadd.reduce();
            if(xs.m<0)
            {
                return -xs;
            }
            else
            {
                return Extended::Real();
            }
        }

        Extended::Real ConservationLaw:: excess(const readable<Extended::Real> &C,
                                                Extended::Adder                &xadd,
                                                const IndexLevel               level) const
        {
            xadd.resume(size);
            const Actor          *ac = head;

            for(;ac;ac=ac->next )
            {
                Extended::Real p =   C[ (**ac).indx[level] ];
                p *= ac->xn;
                xadd.append(p);
            }
            const Extended::Real xs = xadd.reduce();
            if(xs.m<0)
            {
                return -xs;
            }
            else
            {
                return Extended::Real();
            }
        }

    }

}
