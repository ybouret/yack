
#include "yack/ios/encoder.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/arith/align.hpp"
#include <iostream>

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


        size_t encoder:: upack64(ostream &os, uint64_t x)
        {
            const size_t qword_size = sizeof(uint64_t);
            const size_t qword_info = ilog2<qword_size>::value;
            const size_t xbits = bits_for(x);

            if(xbits<=0)
            {
                os.write(0);
                return 1;
            }
            else
            {
                const size_t total_bits = qword_info + xbits;
                const size_t total_size = YACK_ALIGN_ON(8,total_bits) >> 3;
                assert(total_size>=1);
                assert(total_size<=9);
                const uint8_t header = uint8_t(total_size-1); //<< (8-qword_info);

                std::cerr << "size:   " << total_size << std::endl;
                std::cerr << "header: " << unsigned(header) << std::endl;
                return 0;
            }

        }
    }
}

