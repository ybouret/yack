
#include "yack/info/modulation.hpp"

namespace yack
{
    namespace information
    {
        modulation:: ~modulation() throw()
        {
        }

        modulation:: modulation() throw()
        {
            
        }

        uint8_t modulation:: call(const uint8_t x, void *args) throw()
        {
            assert(args);
            return static_cast<modulation *>(args)->alter(x);
        }

        void *modulation:: self() throw()
        {
            return this;
        }

    }
}
