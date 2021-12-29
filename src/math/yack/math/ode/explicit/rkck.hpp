

//! \file

#ifndef YACK_ODE_EXPLICIT_RKCK_INCLUDED
#define YACK_ODE_EXPLICIT_RKCK_INCLUDED 1

#include "yack/math/ode/explicit/rkcs.hpp"
#include "yack/math/ode/explicit/rk45/cash-karp.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            template <typename T>
            struct rkck
            {
                typedef rkcs<T,cash_karp> device;
            };
        }

    }
}

#endif
