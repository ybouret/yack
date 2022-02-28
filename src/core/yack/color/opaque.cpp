
#include "yack/color/opaque.hpp"

namespace yack
{
    namespace color
    {

        template <>
        const uint8_t opaque<uint8_t>::value = 0xff;

        template <>
        const float   opaque<float>::value = 1.0f;

        template <>
        const double   opaque<double>::value = 1.0;


        template <>
        const long double   opaque<long double>::value = 1.0L;



    }

}

