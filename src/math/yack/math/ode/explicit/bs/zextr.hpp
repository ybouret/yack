
//! \file

#ifndef YACK_ODE_EXPLICIT_BS_ZEXTR_INCLUDED
#define YACK_ODE_EXPLICIT_BS_ZEXTR_INCLUDED 1

#include "yack/math/ode/types.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {

            //__________________________________________________________________
            //
            //
            //! zero-extrapolation of vectors
            //
            //__________________________________________________________________
            template <typename T>
            class zextr  : public object, public counted, public arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef arrays_of<T>   tableaux; //!< alias
                typedef arc_ptr<zextr> pointer;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~zextr() throw(); //!< cleanup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! compute yz and dz from a new estimate (xest,yest)
                virtual void operator()(const size_t       iest,
                                        const T            xest,
                                        const readable<T> &yest,
                                        writable<T>       &yz,
                                        writable<T>       &dy,
                                        writable<T>       &x,
                                        matrix<T>         &d) = 0;

            protected:
                explicit zextr(const size_t num_arrays); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(zextr);
            };



        }

    }

}

#endif

