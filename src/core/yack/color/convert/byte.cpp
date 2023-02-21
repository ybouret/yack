
#include "yack/color/convert.hpp"
#include "yack/color/unit-real.hpp"
#include "yack/color/rgba.hpp"

namespace yack
{
    namespace color
    {

        template <>
        void convert<rgba,uint8_t>::make(rgba &c, const uint8_t &u) noexcept
        {
            c.r = c.g = c.b = u;
        }

        template <>
        void convert<rgb,uint8_t>::make(rgb &c, const uint8_t &u) noexcept
        {
            c.r = c.g = c.b = u;
        }

        
    }

}

