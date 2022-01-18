//! \file

#ifndef YACK_ODE_EXPLICIT_RK45_INCLUDED
#define YACK_ODE_EXPLICIT_RK45_INCLUDED 1

#include "yack/math/ode/explicit/step.hpp"
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
            //! base class to manage step pointer
            //
            //__________________________________________________________________
            template <typename T>
            class rk45_ : public virtual entity
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~rk45_()               throw(); //!< cleanup
                explicit rk45_(rk45_step<T> *) throw(); //!< setup from new engine
                explicit rk45_(const typename rk45_step<T>::pointer &) throw(); //!< setup from shared engine

                //______________________________________________________________
                //
                // member
                //______________________________________________________________
                typename rk45_step<T>::pointer move; //!< the step/move implementation

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk45_);

            };

            //! helper to create rk45
#define YACK_RK45_CTOR()  \
rk45_<T>(eng),            \
explicit_step<T>(2),      \
yerr( this->next() ),     \
ytmp( this->next() )


            //__________________________________________________________________
            //
            //
            //! generic 4/5 adaptive step
            //
            //__________________________________________________________________
            template <typename T>
            class rk45 : public rk45_<T>, public explicit_step<T>
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
                virtual ~rk45() throw();          //!< cleanup

                //! setup from new engine
                explicit rk45(rk45_step<T> *eng) : YACK_RK45_CTOR() {}

                //! setup from shared engine
                explicit rk45(const typename rk45_step<T>::pointer &eng) :YACK_RK45_CTOR() {}

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

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
                                        callback          *proc);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk45);
                array_type &yerr;
                array_type &ytmp;
            };

        }

    }
}

#endif
