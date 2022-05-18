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

        template <>
        void broker_blur:: fuzz<uint8_t>(pixmap<uint8_t>       &target,
                                         const pixmap<uint8_t> &source,
                                         broker                &device,
                                         const blurring        &B)
        {
            apply<uint8_t,uint8_t,1>(target,source,device,B);
        }

        template <>
        void broker_blur:: fuzz<float>(pixmap<float>       &target,
                                       const pixmap<float> &source,
                                       broker              &device,
                                       const blurring      &B)
        {
            apply<float,float,1>(target,source,device,B);
        }


    }
}
