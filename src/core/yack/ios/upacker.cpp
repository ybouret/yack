
#include "yack/ios/upacker.hpp"
#include "yack/ios/ostream.hpp"
#include "yack/arith/base2.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/hexa.hpp"

#include <iostream>
namespace yack
{
    namespace ios
    {
        
        size_t upacker:: encode(uint8_t b[], uint64_t qw) throw()
        {
            if(qw<=0)
            {
                return 0;
            }
            else
            {
                const size_t data_bits = bits_for(qw);
                const size_t info_bits = 4;
                const size_t full_bits = data_bits+info_bits;
                const size_t num_bytes = YACK_ALIGN_ON(8,full_bits)>>3;
                const size_t semibytes = YACK_ALIGN_ON(4,data_bits)>>2;
                assert(num_bytes>=1);
                assert(num_bytes<=9);
                uint8_t q[32] = {
                    uint8_t(num_bytes),0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0
                };


                std::cerr << "bytes  : " << num_bytes << std::endl;
                std::cerr << "semi   : " << semibytes << std::endl;

                // prepare all quartets
                std::cerr << "[ " << hexa::uppercase_char[ q[0] ];
                for(size_t i=1;i<=semibytes;++i)
                {
                    q[i] = uint8_t(qw&0xf);
                    qw >>= 4;
                    std::cerr << ' ' << hexa::uppercase_char[q[i]];
                }
                std::cerr << " ]" << std::endl;

                b[0] = (q[0]  << 4) | q[1];
                b[1] = (q[2]  << 4) | q[3];
                b[2] = (q[4]  << 4) | q[5];
                b[3] = (q[6]  << 4) | q[7];
                b[4] = (q[8]  << 4) | q[9];
                b[5] = (q[10] << 4) | q[11];
                b[6] = (q[12] << 4) | q[13];
                b[7] = (q[14] << 4) | q[15];
                b[8] = (q[16] << 4) | q[17];

                std::cerr << "(";
                for(size_t i=0;i<num_bytes;++i) std::cerr << ' ' << hexa::uppercase_text[b[i]];
                std::cerr << " )" << std::endl;


                return num_bytes;
            }
        }

    }
    
}
