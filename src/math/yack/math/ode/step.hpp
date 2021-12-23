//! \file

#ifndef YACK_ODE_STEP_INCLUDED
#define YACK_ODE_STEP_INCLUDED 1

#include "yack/math/ode/types.hpp"
#include "yack/sequence/arrays.hpp"

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
            class step : public arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation equation; //!< alias
                typedef typename named<T>::callback callback; //!< alias
                typedef arrays_of<T>                tableaux; //!< alias

                //______________________________________________________________
                //
                //! interface
                //______________________________________________________________
                virtual void run(const readable<T> &y,
                                 const readable<T> &dydx,
                                 const T            x,
                                 const T            h,
                                 writable<T>       &yout,
                                 writable<T>       &yerr,
                                 equation          &drvs,
                                 callback          *proc) = 0;
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~step() throw(); //!< cleanup

            protected:
                explicit step(const size_t num_arrays); //!< setup

            };

        }

    }
}

#endif
