
#include "yack/info/modulation.hpp"

namespace yack
{
    namespace information
    {
        modulation:: ~modulation() noexcept
        {
        }

        modulation:: modulation() noexcept
        {
            
        }

        uint8_t modulation:: call(const uint8_t x, void *args) noexcept
        {
            assert(args);
            return static_cast<modulation *>(args)->alter(x);
        }

        void *modulation:: self() noexcept
        {
            return this;
        }

    }
}
