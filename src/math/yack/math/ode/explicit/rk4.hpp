//! \file
#ifndef YACK_ODE_RK4_INCLUDED
#define YACK_ODE_RK4_INCLUDED 1

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
            //! Runge-Kutta 4 interface
            //
            //__________________________________________________________________
            template <typename T>
            class rk4 : public arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation   equation; //!< alias
                typedef arrays_of<T>                  tableaux; //!< alias
                typedef typename tableaux::array_type array;    //!< alias
                using tableaux::next;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit rk4(const size_t nvar=0); //!< setup with optional nvar
                virtual ~rk4() throw();            //!< cleanup

                //______________________________________________________________
                //
                //! integration method
                //______________________________________________________________
                void operator()(writable<T> &y0,
                                const T      x0,
                                const T      x1,
                                equation    &eq);

                //______________________________________________________________
                //
                //! members
                //______________________________________________________________
                array &k1; //!< helper
                array &k2; //!< helper
                array &k3; //!< helper
                array &k4; //!< helper
                array &yt; //!< helper

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rk4);
            };

        }

    }
}

#endif

