
#include "yack/chem/eqs/component.hpp"

namespace yack
{
    namespace Chemical
    {
        Component:: ~Component() noexcept
        {
        }

        Component:: Component(const int n, const Species &s) noexcept :
        nu(n),
        xn(nu),
        sp(s)
        {
            assert(nu!=0);
        }

        const Species & Component:: operator*() const noexcept
        {
            return sp;
        }

        const string  & Component:: key() const noexcept
        {
            return sp.name;
        }

    }

}

