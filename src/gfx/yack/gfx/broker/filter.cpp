#include "yack/gfx/broker/filter.hpp"

namespace yack
{
    namespace graphic
    {
        void broker_filter:: initialize(broker &device)
        {
            device->build<real_t>(2);
        }

        filter::real_t broker_filter:: finalize(const broker &device) noexcept
        {
            const concurrent::loop &eng  = *device;
            size_t                  idx  = eng.size();
            groove                 &ini  = *eng[idx];
            real_t                  vmax = ini.as<real_t>();
            while(--idx>0)
            {
                const tiles  &part = device[idx]; if(part.size()<=0) break;
                const groove &curr = *eng[idx];
                const real_t  temp = curr.as<real_t>();
                if(vmax<temp) vmax = temp;
            }
            return vmax;
        }
    }

}
