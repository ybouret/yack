#include "yack/color/convert.hpp"
#include "yack/color/unit-real.hpp"
#include "yack/color/rgba.hpp"

namespace yack
{
    namespace color
    {

        template <>
        void convert<rgba,float>::make(rgba &c, const float &f) throw()
        {
            c.r = c.g = c.b = unit_real<float>::to_byte(f);
        }
        
        template <>
        void convert<rgb,float>::make(rgb &c, const float &f) throw()
        {
            c.r = c.g = c.b = unit_real<float>::to_byte(f);
        }


        template <>
        void convert<uint8_t,float>::make(uint8_t &c, const float &f) throw()
        {
            c = unit_real<float>::to_byte(f);
        }

    }

}

