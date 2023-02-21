
namespace yack
{
    namespace math
    {

        namespace ode
        {

            template <>
            appliance<real_t>:: ~appliance() noexcept
            {
            }

            template <>
            void appliance<real_t>:: operator()(writable<real_t> &ystart,
                                                const real_t      x1,
                                                const real_t      x2,
                                                real_t           &h1,
                                                equation          drvs,
                                                callback         *proc)
            {
                (*drv)( *dev, ystart,x1,x2,eps,h1,drvs,proc) ;
            }

            template <>
            appliance<real_t>:: appliance(step<real_t> *forward,
                                          const real_t  epsilon) :
            dev( forward ),
            drv( new driver_t() ),
            eps( epsilon )
            {
            }
            
        }

    }

}
