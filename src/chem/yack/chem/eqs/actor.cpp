
#include "yack/chem/eqs/actor.hpp"

namespace yack
{
    namespace chemical
    {
        actor:: ~actor() noexcept
        {
        }

        actor:: actor(const species &s, const unsigned n) noexcept :
        object(),
        nu(n),
        nm(nu-1),
        sp(s),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        actor:: actor(const actor &other) noexcept :
        object(),
        nu(other.nu),
        nm(other.nm),
        sp(other.sp),
        next(0),
        prev(0)
        {
        }

        const species & actor:: operator*() const noexcept
        {
            return sp;
        }

        void actor:: display_first(std::ostream &os) const
        {
            if(nu>1) os << nu;
            os << sp;
        }



        void actor:: display_extra(std::ostream &os) const
        {
            os << '+';
            if(nu>1) os << nu;
            os << sp;
        }

        string actor:: to_string_first() const
        {
            return nu>1 ? vformat("%u[%s]",nu,sp.name()) : vformat("[%s]",sp.name());
        }

        string actor:: to_string_extra() const
        {
            return nu>1 ? vformat("+%u[%s]",nu,sp.name()) : vformat("+[%s]",sp.name());
        }

        void actor:: mass_action(const index_level       I,
                                 const readable<double> &C,
                                 cameo::mul<double>     &xmul) const
        {
            xmul.push( C[sp.indx[I]], nu);
        }

        size_t  actor:: grad_action(const index_level       I,
                                    const readable<double> &C,
                                    cameo::mul<double>     &xmul) const
        {
            const size_t i = sp.indx[I];

            if(nm>0)
            {
                assert(nu>1);
                xmul.push(nu);
                xmul.push(C[i],nm);
            }

            return i;
        }

    }

}

