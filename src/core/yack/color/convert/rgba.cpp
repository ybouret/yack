#include "yack/color/convert.hpp"
#include "yack/color/unit-real.hpp"
#include "yack/color/rgba.hpp"

namespace yack
{
    namespace color
    {


        template <>
        void convert<float,rgba>::make(float &f, const rgba &c) noexcept
        {
            f = convtable::f[unsigned(c.r)+unsigned(c.g)+unsigned(c.b)];
        }

        template <>
        void convert<rgb,rgba>::make(rgb &f, const rgba &c) noexcept
        {
            f = c;
        }

        template <>
        void convert<uint8_t,rgba>::make(uint8_t &f, const rgba &c) noexcept
        {
            f = convtable::u[unsigned(c.r)+unsigned(c.g)+unsigned(c.b)];
        }
    }
}
