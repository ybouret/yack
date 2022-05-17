#include "yack/gfx/broker/filter.hpp"
namespace yack
{
    namespace graphic
    {
        void broker_filter:: initialize(broker &device)
        {
            (*device).build<filter::real_t>();
        }

        filter::real_t broker_filter:: finalize(const broker &device) throw()
        {
            real_t gmax = 0;
            const concurrent::loop &eng = *device;
            for(size_t i=eng.size();i>0;--i)
            {
                const groove &dat  = *eng[i];
                const real_t  tmp  = dat.as<real_t>();
                if(gmax<tmp) gmax = tmp;
            }
            return gmax;
        }
    }

}
