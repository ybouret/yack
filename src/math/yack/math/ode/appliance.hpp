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
            template <typename T>
            class appliance : public object, public counted
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
                typedef step<T>                       device_t; //!< alias
                typedef arc_ptr<device_t>             device_p; //!< alias
                typedef arc_ptr<appliance>            pointer;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~appliance() throw();

                //! setup
                explicit appliance(step<T> *forward, const T  epsilon);

                //______________________________________________________________
                //
                //! integration
                //______________________________________________________________
                void operator()(writable<T> &ystart,
                                const T      x1,
                                const T      x2,
                                T           &h1,
                                equation     drvs,
                                callback    *proc=NULL);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                device_p dev; //!< device
                driver_p drv; //!< driver
                T        eps; //!< tolerance

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(appliance);
            };

            //__________________________________________________________________
            //
            //
            //! specialized appliance
            //
            //__________________________________________________________________
            template <typename T, template <typename> class STEP>
            class app : public appliance<T>
            {
            public:

                //! cleanup
                inline virtual ~app() throw() {}

                //! setup
                inline explicit app(const T epsilon) :
                appliance<T>( new typename STEP<T>::device(), epsilon)
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(app);
            };


        }

    }

}

#endif

