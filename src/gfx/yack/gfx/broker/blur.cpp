#include "yack/gfx/broker/blur.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {
        template < >
        void broker_blur:: fuzz<rgba>(pixmap<rgba>       &target,
                                      const pixmap<rgba> &source,
                                      broker             &device,
                                      const blurring     &B)
        {
            apply<rgba,uint8_t,3>(target,source,device,B);
        }

        template < >
        void broker_blur:: fuzz<rgb>(pixmap<rgb>       &target,
                                     const pixmap<rgb> &source,
                                     broker             &device,
                                     const blurring     &B)
        {
            apply<rgb,uint8_t,3>(target,source,device,B);
        }


    }
}
