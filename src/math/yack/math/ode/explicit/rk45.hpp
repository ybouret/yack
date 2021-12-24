

//! \file

#ifndef YACK_ODE_EXPLICIT_RK45_INCLUDED
#define YACK_ODE_EXPLICIT_RK45_INCLUDED 1

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
            //! generic step
            //
            //__________________________________________________________________
            template <typename T>
            class rk45 : public explicit_step<T>
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
                // C++
                //______________________________________________________________
                virtual ~rk45() throw(); //!< cleanup
                explicit rk45();         //!< setup


                //! interface to be driven
                virtual bool operator()(writable<T>       &y,
                                        const readable<T> &dydx,
                                        T                 &x,
                                        const T            htry,
                                        const T            eps,
                                        const readable<T> &yscal,
                                        T                 &hdid,
                                        T                 &hnxt,
                                        equation          &drvs,
                                        callback          *proc);


                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk45);
                array_type &yerr, &ytmp;
            };

        }

    }
}

#endif
