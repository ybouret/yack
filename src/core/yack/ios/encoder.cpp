
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace ios
    {
        void encoder:: put(ostream &os, const uint8_t &x)
        {
            os.write( char(x) );
        }

        void encoder:: put(ostream &os, const uint16_t &x)
        {
            const uint8_t arr[2] =
            {
                uint8_t((x>>8) & 0xff),
                uint8_t(x&0xff)
            };
            os.frame(arr,2);
        }

        void encoder:: put(ostream &os, const uint32_t &x)
        {
            const uint8_t arr[4] =
            {
                uint8_t((x>>24) & 0xff ),
                uint8_t((x>>16) & 0xff),
                uint8_t((x>>8) & 0xff),
                uint8_t(x&0xff)
            };
            os.frame(arr,4);
        }

        void encoder:: put(ostream &os, const uint64_t &x)
        {
            static const uint64_t mask = YACK_U64(0xff);
            const uint8_t arr[8] =
            {
                uint8_t((x>>56)&mask),
                uint8_t((x>>48)&mask),
                uint8_t((x>>40)&mask),
                uint8_t((x>>32)&mask),
                uint8_t((x>>24)&mask),
                uint8_t((x>>16)&mask),
                uint8_t((x>>8) &mask),
                uint8_t(x&mask)
            };
            os.frame(arr,8);
        }
    }
}

