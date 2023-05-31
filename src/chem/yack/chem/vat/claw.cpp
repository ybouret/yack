
#include "yack/chem/vat/claw.hpp"

namespace yack
{
    namespace Chemical
    {
        ConservationLaw:: ConservationLaw() :
        Q(),
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
            Extended::CoreList &xl = coerce(Q); assert(0==xl.size);
            try
            {
                for(const Actor *a=head;a;a=a->next)
                {
                    const XReal xr = Extended::Send(a->nu);
                    xl << xr;
                    xadd.append(xr);
                }
                coerce(Q2) = xadd.reduce();
            }
            catch(...)
            {
                xl.clear();
                throw;
            }
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

        XReal ConservationLaw:: excess(const readable<double> &C,
                                       Extended::Adder        &xadd,
                                       const IndexLevel        level) const
        {
            assert(size==Q.size);
            xadd.resume(size);
            const Actor          *ac = head;
            const Extended::Node *nu = Q.head;

            for(;ac;ac=ac->next,nu=nu->next)
            {
                assert(nu!=NULL);
                XReal p = Extended::Send( C[ (**ac).indx[level] ] );
                p *= **nu;
                xadd.append(p);
            }
            const XReal xs = xadd.reduce();
            if(xs.m<0)
            {
                return -xs;
            }
            else
            {
                return XReal();
            }
        }

    }

}
