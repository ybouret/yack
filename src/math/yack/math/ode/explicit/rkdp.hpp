


//! \file

#ifndef YACK_ODE_EXPLICIT_RKDP_INCLUDED
#define YACK_ODE_EXPLICIT_RKDP_INCLUDED 1

#include "yack/math/ode/explicit/rkcs.hpp"
#include "yack/math/ode/explicit/rk45/dormand-prince.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! Runge-Kutta Dormand-Prince
            //
            //__________________________________________________________________
            template <typename T>
            struct rkdp
            {
                typedef rkcs<T,dormand_prince> device; //!< alias
            };
        }

    }
}

#endif
