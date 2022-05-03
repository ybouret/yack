
#include "yack/gfx/broker/histogram.hpp"

namespace yack
{
    namespace graphic
    {

        void broker_histogram:: initialize(histogram &hist,const broker &device)
        {
            hist.reset();
            (*device).build<histogram>();
        }

        void broker_histogram:: finalize(histogram &hist, const broker &device)  
        {
            const concurrent::loop &ops = *device;
            for(size_t i=ops.size();i>0;--i)
            {
                const groove &data = *ops[i];
                assert(data.is<histogram>());
                hist.merge( data.as<histogram>() );
            }
        }


    }

}


