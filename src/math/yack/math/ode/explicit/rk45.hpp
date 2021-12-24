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


            
            template <typename T>
            class rk45_
            {
            public:
                virtual ~rk45_() throw() {}

                explicit rk45_( rk45_step<T> *eng ) throw() : move(eng) {}
                explicit rk45_( const typename rk45_step<T>::pointer &eng ) throw() : move(eng) {}

                typename rk45_step<T>::pointer move;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk45_);

            };

#define YACK_RK45_CTOR()  \
rk45_<T>(eng),\
explicit_step<T>(2),\
yerr( this->next() ),\
ytmp( this->next() )


            //__________________________________________________________________
            //
            //
            //! generic step
            //
            //__________________________________________________________________
            template <typename T>
            class rk45 : public rk45_<T>, explicit_step<T>
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


                explicit rk45(rk45_step<T> *eng) : YACK_RK45_CTOR() {}
                explicit rk45(const typename rk45_step<T>::pointer &eng) :YACK_RK45_CTOR() {}
                

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
                array_type &yerr;
                array_type  &ytmp;
            };

        }

    }
}

#endif
