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
            //! generic step
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
                typedef typename named<T>::equation equation; //!< alias
                typedef typename named<T>::callback callback; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~explicit_step() noexcept;                //!< cleanup

            protected:
                explicit explicit_step(const size_t num_arrays); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(explicit_step);
            };

        }

    }
}

#endif
