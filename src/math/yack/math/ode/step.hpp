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
            class step : public virtual entity, public arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation equation; //!< alias
                typedef typename named<T>::callback callback; //!< alias
                typedef arrays_of<T>                tableaux; //!< alias

                //! interface to be driven
                virtual void operator()(writable<T>       &y,
                                        const readable<T> &dydx,
                                        T                 &x,
                                        const T            htry,
                                        const T            eps,
                                        const readable<T> &yscal,
                                        T                 &hdid,
                                        T                 &hnxt,
                                        equation          &drvs,
                                        callback          *proc) = 0;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~step() throw(); //!< cleanup

            protected:
                explicit step(const size_t num_arrays); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(step);
            };

        }

    }
}

#endif
