

//! \file

#ifndef YACK_ODE_EXPLICIT_CASH_KARP_INCLUDED
#define YACK_ODE_EXPLICIT_CASH_KARP_INCLUDED 1

#include "yack/math/ode/explicit/step.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! Cash-Karp 4/5 step
            //
            //__________________________________________________________________
            template <typename T>
            class cash_karp : public explicit_step<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename step<T>::equation   equation;    //!< alias
                typedef typename step<T>::callback   callback;    //!< alias
                typedef typename step<T>::array_type array_type;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~cash_karp() throw(); //!< cleanup
                explicit cash_karp();         //!< setup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! build output from initial guess
                virtual void run(const readable<T> &y,
                                 const readable<T> &dydx,
                                 const T            x,
                                 const T            h,
                                 writable<T>       &yout,
                                 writable<T>       &yerr,
                                 equation          &drvs,
                                 callback          *proc);

            private:
                array_type &ak2, &ak3, &ak4, &ak5, &ak6, &ytemp;
            };

        }

    }
}

#endif
