
#include "yack/gfx/broker/normalize.hpp"
#include "yack/color/unit-real.hpp"

namespace yack
{
    namespace graphic
    {

        template < >
        void normalize_pixel<uint8_t>(uint8_t &v, const uint8_t vmax) throw()
        {
            if(v>=vmax)
            {
                v = vmax;
            }
            else
            {
                v = color::unit_real<float>::to_byte( float(v)/vmax );
            }
        }

        template <>
        void normalize_pixel<float>(float &v, const float vmax) throw()
        {
            if(v<=0)
            {
                v=0;
            }
            else
            {
                if(v>=vmax)
                {
                    v=vmax;
                }
                else
                {
                    v /= vmax;
                }
            }
        }


    }

}

