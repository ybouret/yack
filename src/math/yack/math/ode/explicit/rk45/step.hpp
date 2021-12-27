
//! \file

#ifndef YACK_ODE_EXPLICIT_RK45_STEP_INCLUDED
#define YACK_ODE_EXPLICIT_RK45_STEP_INCLUDED 1

#include "yack/math/ode/types.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {

            //__________________________________________________________________
            //
            //
            //! interface to a rk45 step
            //
            //__________________________________________________________________
            template <typename T>
            class rk45_step : public object, public counted, public arrays_of<T>
            {
            public:
                typedef typename named<T>::equation    equation;    //!< alias
                typedef typename named<T>::callback    callback;    //!< alias
                typedef arc_ptr<rk45_step>             pointer;     //!< alias
                
                virtual ~rk45_step() throw();

                virtual void operator()(const readable<T> &y,
                                        const readable<T> &dydx,
                                        const T            x,
                                        const T            h,
                                        writable<T>       &yout,
                                        writable<T>       &yerr,
                                        equation          &drvs,
                                        callback          *proc) = 0;

            protected:
                explicit rk45_step(const size_t n);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk45_step);
            };

        }

    }
}

#endif
