
#include "yack/gfx/broker/normalize.hpp"
#include "yack/color/unit-real.hpp"

namespace yack
{
    namespace graphic
    {

        template < >
        void broker_normalize:: procedure<uint8_t>:: operator()(uint8_t &v) const throw()
        {
            if(v<=vmin)
            {
                v = 0;
            }
            else
            {
                if(v>=vmax)
                {
                    v = 255;
                }
                else
                {
                    assert(scal>0);
                    v = color::unit_real<float>::to_byte( float(v-vmin)/scal );
                }
            }
        }

        template < >
        void broker_normalize:: procedure<float>:: operator()(float &v) const throw()
        {
            if(v<=vmin)
            {
                v = 0;
            }
            else
            {
                if(v>=vmax)
                {
                    v = 1.0f;
                }
                else
                {
                    assert(scal>0);
                    v = (v-vmin)/scal;
                }
            }
        }



    }

}

