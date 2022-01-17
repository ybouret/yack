
//! \file

#ifndef YACK_ODE_EXPLICIT_MIDPOINT_INCLUDED
#define YACK_ODE_EXPLICIT_MIDPOINT_INCLUDED 1

#include "yack/math/ode/types.hpp"
#include "yack/sequence/arrays.hpp"
namespace yack
{
    namespace math
    {

        namespace ode
        {


            //! midpoint method for Burlich-Stoer
            template <typename T>
            class midpoint : public arrays_of<T>
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
                explicit midpoint();
                virtual ~midpoint() throw();

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! evaluate yout from nstep midpoints
                void operator()(const readable<T> &y,
                                const readable<T> &dydx,
                                const T            xs,
                                const T            htot,
                                const size_t       nstep,
                                writable<T>       &yout,
                                equation          &drvs,
                                callback          *proc);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(midpoint);
                array_type &yn, &ym;
            };
        }

    }
}

#endif
