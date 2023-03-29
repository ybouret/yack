
#include "yack/chem/eqs/component.hpp"

namespace yack
{
    namespace chemical
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

