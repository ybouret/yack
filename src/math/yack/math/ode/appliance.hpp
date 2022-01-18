//! \file

#ifndef YACK_ODE_APPLIANCE_INCLUDED
#define YACK_ODE_APPLIANCE_INCLUDED 1

#include "yack/math/ode/driver.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    namespace math
    {

        namespace ode
        {

            //__________________________________________________________________
            //
            //
            //! make an appliance to integrate equation
            //
            //__________________________________________________________________
            template <
            typename T,
            template <typename> class STEP
            >
            class appliance
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef typename named<T>::equation   equation; //!< alias
                typedef typename named<T>::callback   callback; //!< alias
                typedef driver<T>                     driver_t; //!< alias
                typedef arc_ptr<driver_t >            driver_p; //!< alias
                typedef typename STEP<T>::device      device_t; //!< alias
                typedef arc_ptr<device_t>             device_p; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~appliance() throw() {}

                //! setup
                inline explicit appliance(const T epsilon=1e-4) :
                drv( new driver_t() ),
                dev( new device_t() ),
                eps( epsilon )
                {
                }

                //______________________________________________________________
                //
                //! integration
                //______________________________________________________________
                inline void operator()(writable<T> &ystart,
                                       const T      x1,
                                       const T      x2,
                                       T           &h1,
                                       equation     drvs,
                                       callback    *proc)
                {
                    (*drv)( *dev, ystart,x1,x2,eps,h1,drvs,proc) ;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                driver_p drv; //!< driver
                device_p dev; //!< device
                T        eps; //!< tolerance

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(appliance);
            };

        }

    }

}

#endif

