#include "yack/color/convert.hpp"
#include "yack/color/unit-real.hpp"
#include "yack/color/rgba.hpp"

namespace yack
{
    namespace color
    {
        template <>
        template <>
        void convert<rgba>::to<float>(float &f, const rgba &c) throw()
        {
            f = conversion::ftable[c.r+c.g+c.b];
        }

        template <>
        template <>
        void convert<rgba>::to<rgb>(rgb &f, const rgba &c) throw()
        {
            f = c;
        }

        template <>
        template <>
        void convert<rgba>::to<uint8_t>(uint8_t &f, const rgba &c) throw()
        {
            f = conversion::utable[c.r+c.g+c.b];
        }
    }
}
