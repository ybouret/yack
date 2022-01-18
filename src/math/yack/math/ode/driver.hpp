//! \file

#ifndef YACK_ODE_DRIVER_INCLUDED
#define YACK_ODE_DRIVER_INCLUDED 1

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
            //! driver for a step
            //
            //__________________________________________________________________
            template <typename T>
            class driver : public entity, arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation   equation;   //!< alias
                typedef typename named<T>::callback   callback;   //!< alias
                typedef arrays_of<T>                  tableaux;   //!< alias
                typedef typename tableaux::array_type array_type; //!< alias

                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                explicit driver();
                virtual ~driver() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! go from x1 to x2
                void operator()(step<T>     &moving,
                                writable<T> &ystart,
                                const T      x1,
                                const T      x2,
                                const T      eps,
                                T           &h1,
                                equation     drvs,
                                callback    *proc);

                size_t  nfit; //!< for one step from x1 to x2
                size_t  nbad; //!< for one step from x1 to x2
                const T tiny; //!< minimal scaling value

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(driver);
                array_type &y, &dydx, &yscal;

            };
        }

    }

}

#endif
