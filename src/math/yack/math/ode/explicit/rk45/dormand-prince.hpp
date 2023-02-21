//! \file

#ifndef YACK_ODE_EXPLICIT_DORMAND_PRINCE_INCLUDED
#define YACK_ODE_EXPLICIT_DORMAND_PRINCE_INCLUDED 1

#include "yack/math/ode/explicit/rk45/step.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! Dormand-Prince 4/5 step
            //
            //__________________________________________________________________
            template <typename T>
            class dormand_prince : public rk45_step<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation    equation;    //!< alias
                typedef typename named<T>::callback    callback;    //!< alias
                typedef arrays_of<T>                   tableaux;    //!< alias
                typedef typename tableaux::array_type  array_type;  //!< alias
                typedef rk45_step<T>                   base_type;   //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~dormand_prince() noexcept; //!< cleanup
                explicit dormand_prince();         //!< setup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! build output from initial guess
                virtual void operator()(const readable<T> &y,
                                        const readable<T> &dydx,
                                        const T            x,
                                        const T            h,
                                        writable<T>       &yout,
                                        writable<T>       &yerr,
                                        equation          &drvs,
                                        callback          *proc);

            private:
                array_type &ak2, &ak3, &ak4, &ak5, &ak6, &ak7, &ytmp;
            };

        }

    }
}

#endif
