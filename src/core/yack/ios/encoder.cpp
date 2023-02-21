
#include "yack/ios/encoder.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/hexa.hpp"

#include "yack/ios/ostream.hpp"

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

        size_t encoder:: upack(uint8_t *b, uint64_t qw) noexcept
        {
            assert(b!=NULL);
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



                //______________________________________________________________
                //
                // prepare all quartets
                //______________________________________________________________
                for(size_t i=1;i<=semibytes;++i)
                {
                    q[i] = uint8_t(qw&0xf);
                    qw >>= 4;
                }

                //______________________________________________________________
                //
                // merge all quartets
                //______________________________________________________________
                b[0] = (q[0]  << 4) | q[1];
                b[1] = (q[2]  << 4) | q[3];
                b[2] = (q[4]  << 4) | q[5];
                b[3] = (q[6]  << 4) | q[7];
                b[4] = (q[8]  << 4) | q[9];
                b[5] = (q[10] << 4) | q[11];
                b[6] = (q[12] << 4) | q[13];
                b[7] = (q[14] << 4) | q[15];
                b[8] = (q[16] << 4) | q[17];

                
                return num_bytes;
            }
        }


        size_t encoder:: serialize64(ostream &os, const uint64_t qw)
        {
            if(qw<=0)
            {
                os.write(0);
                return 1;
            }
            else
            {
                uint8_t       b[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                const size_t  size  = upack(b,qw);
                os.frame(b,size);
                return size;
            }
        }

        
        size_t encoder::serialize(ostream &os, const void *block_addr, const size_t block_size)
        {
            if(block_size)
            {
                assert(block_addr);
                size_t nw = serialize(os,block_size);
                os.frame(block_addr,block_size);
                return nw + block_size;
            }
            else
            {
                os.write(0);
                return 1;
            }
        }



    }
}

#include "yack/ios/fmt/hexa.hpp"

namespace yack
{
    namespace ios
    {
        size_t encoder:: addr2hexa(ostream &os, const void *addr, const char pfx )
        {
            // prefix
            const size_t nw  = (pfx!=0) ? emit<uint8_t>(os,pfx) : 0;
            char         ch[16] = { 0 };
            const size_t nc  = hexa::text(ch,addr);
            os.frame(ch,nc);
            return nw+nc;
        }

    }

}
