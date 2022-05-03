
#include "yack/gfx/broker/histogram.hpp"

namespace yack
{
    namespace graphic
    {

        broker_histogram:: ~broker_histogram() throw()
        {

        }

        broker_histogram:: broker_histogram(const broker &device) :
        self(),
        hist(device.size())
        {
        }

        void broker_histogram:: initialize() throw()
        {
            self.reset();
            for(size_t i=hist.size();i>0;--i)
            {
                hist[i].reset();
            }
        }

        void broker_histogram:: finalize()   throw()
        {
            for(size_t i=hist.size();i>0;--i)
            {
                self.merge(hist[i]);
            }
        }

    }

}


