
#include "yack/aqueous/eqs/component.hpp"

namespace yack
{
    namespace aqueous
    {
        component:: ~component() noexcept {}


        component:: component(const species &s, const int n) noexcept :
        object(),
        counted(),
        nu(n),
        sp(s)
        {
        }

        const string  & component:: key() const noexcept
        {
            return sp.name;
        }

        const species & component:: operator*() const noexcept
        {
            return sp;
        }


    }

}

