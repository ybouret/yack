
//! \file

#ifndef YACK_ODE_EXPLICIT_RKCS_INCLUDED
#define YACK_ODE_EXPLICIT_RKCS_INCLUDED 1

#include "yack/math/ode/explicit/rk45.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {

            //! Runge-Kutta 4/5 Control Step
            template <typename T,
            template <typename> class ENGINE>
            class rkcs : public rk45<T>
            {
            public:
                typedef ENGINE<T> engine;

                inline virtual ~rkcs() throw() {}
                inline explicit rkcs() : rk45<T>( new engine() ) {}
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rkcs);
            };

        }

    }
}

#endif
