
#include "yack/aqueous/actor.hpp"

namespace yack
{
    namespace aqueous
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





    }

}

