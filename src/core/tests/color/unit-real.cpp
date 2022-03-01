
#include "yack/color/unit-real.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(color_unit_real)
{

    for(size_t i=0;i<256;++i)
    {
        {
            const float   f = color::unit_real<float>::table[i];
            const uint8_t u = color::unit_real<float>::to_byte(f);
            YACK_ASSERT(u==i);
        }

        {
            const double  f = color::unit_real<double>::table[i];
            const uint8_t u = color::unit_real<double>::to_byte(f);
            YACK_ASSERT(u==i);
        }

        {
            const long double f = color::unit_real<long double>::table[i];
            const uint8_t     u = color::unit_real<long double>::to_byte(f);
            YACK_ASSERT(u==i);
        }
    }

}
YACK_UDONE()

