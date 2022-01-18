//! \file

#ifndef YACK_ODE_TYPE_INCLUDED
#define YACK_ODE_TYPE_INCLUDED 1

#include "yack/functor.hpp"
#include "yack/container/writable.hpp"
#include "yack/large-object.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {
            //__________________________________________________________________
            //
            //
            //! definitions for differential equations
            //
            //__________________________________________________________________
            template  <typename T>
            struct named
            {
                typedef functor<void,TL3(writable<T> &,T,const readable<T> &)> equation; //!< interface equation
                typedef functor<void,TL2(writable<T> &,T)>                     callback; //!< callback for phase space
            };

            //__________________________________________________________________
            //
            //
            //! base class for ODE classes
            //
            //__________________________________________________________________
            class entity : public large_object, public counted
            {
            public:
                virtual ~entity() throw(); //!< cleanup

            protected:
                explicit entity() throw(); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(entity);
            };
            
        }

    }
}

#endif
