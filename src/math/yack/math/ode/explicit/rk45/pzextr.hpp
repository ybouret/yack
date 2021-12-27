

//! \file

#ifndef YACK_ODE_EXPLICIT_RK45_PZEXTR_INCLUDED
#define YACK_ODE_EXPLICIT_RK45_PZEXTR_INCLUDED 1

#include "yack/math/ode/explicit/rk45/zextr.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! polynomial zero extrapolation
            //
            //__________________________________________________________________
            template <typename T>
            class pzextr  : public zextr<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef arrays_of<T>                  tableaux;   //!< alias
                typedef typename tableaux::array_type array_type; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~pzextr() throw(); //!< cleanup
                explicit pzextr();         //!< setup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! extrapolate using polynomials
                virtual void operator()(const size_t       iest,
                                        const T            xest,
                                        const readable<T> &yest,
                                        writable<T>       &yz,
                                        writable<T>       &dy,
                                        writable<T>       &x,
                                        matrix<T>         &d);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(pzextr);
                array_type &c;
            };



        }

    }

}

#endif

