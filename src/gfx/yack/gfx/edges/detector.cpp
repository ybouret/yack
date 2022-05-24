#include "yack/gfx/edges/detector.hpp"
#include "yack/gfx/broker/filter.hpp"

namespace yack
{

    namespace graphic
    {


        edges:: ~edges() throw()
        {
        }

        edges:: edges(const unit_t w, const unit_t h) :
        pixmap<uint8_t>(w,h),
        origin(w,h),
        gfield(w,h),
        vfield(w,h),
        labels(w,h),
        ridges(),
        gmax(0)
        {
        }


        void edges:: detect(broker              &device,
                            const filters       &deltas)
        {
            device->ensure(1,sizeof(histogram));
            gmax = broker_filter::gradient(gfield,vfield,origin,device,*deltas.X,*deltas.Y);
            const uint8_t up = thinning(device);
            cleaving(device,up);
            tracking(device);
        }
        
        void edges:: tracking(broker &device)
        {
            ridges.build(labels, device, *this, 8);
        }

    }

}
