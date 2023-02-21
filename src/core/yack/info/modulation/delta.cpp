
#include "yack/info/modulation/delta.hpp"
#include <cstring>

namespace yack
{

    namespace information
    {
        delta:: ~delta() noexcept
        {
            last = 0;
        }


        delta:: delta() noexcept : last(0)
        {
        }

        void delta:: reset() noexcept
        {
            last = 0;
        }

    }

}


namespace yack
{

    namespace information
    {

        delta_encoder:: delta_encoder() noexcept : delta()
        {

        }

        delta_encoder:: ~delta_encoder() noexcept
        {
        }

        uint8_t delta_encoder:: alter(const uint8_t x) noexcept
        {
            const uint8_t ans = x-last;
            last = x;
            return ans;
        }
    }

}

namespace yack
{

    namespace information
    {

        delta_decoder:: delta_decoder() noexcept : delta()
        {

        }

        delta_decoder:: ~delta_decoder() noexcept
        {
        }

        uint8_t delta_decoder:: alter(const uint8_t x) noexcept
        {
            const uint8_t ans = x+last;
            last = ans;
            return ans;
        }

    }

}
