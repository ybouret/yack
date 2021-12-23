//! \file

#ifndef YACK_ODE_EXPLICIT_STEP_INCLUDED
#define YACK_ODE_EXPLICIT_STEP_INCLUDED 1

#include "yack/math/ode/step.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! explicit 4/5 step
            //
            //__________________________________________________________________
            template <typename T>
            class explicit_step : public step<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename step<T>::equation equation; //!< alias
                typedef typename step<T>::callback callback; //!< alias
                typedef typename step<T>::tableaux tableaux; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~explicit_step() throw();     //!< cleanup

            protected:
                explicit explicit_step(const size_t); //!< setup

            };

        }

    }
}

#endif
