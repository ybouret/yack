
#include "yack/ios/decoder.hpp"
#include "yack/system/exception.hpp"
#include "yack/arith/base2.hpp"
#include <cerrno>


namespace yack
{
    namespace ios
    {
        bool decoder:: get(istream &is, uint8_t &x)
        {
            return is.query( (char&)x );
        }

        bool decoder:: get(istream &is, uint16_t &x)
        {
            uint8_t arr[2] = { 0, 0 };
            if(2!=is.fetch(arr,2))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1];
                return true;
            }
        }

        bool decoder:: get(istream &is, uint32_t &x)
        {
            uint8_t arr[4] = { 0, 0, 0, 0 };
            if(4!=is.fetch(arr,4))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1]; x <<= 8;
                x |= arr[2]; x <<= 8;
                x |= arr[3];
                return true;
            }
        }

        bool decoder:: get(istream &is, uint64_t &x)
        {
            uint8_t arr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            if(8!=is.fetch(arr,8))
            {
                return false;
            }
            else
            {
                x  = arr[0]; x <<= 8;
                x |= arr[1]; x <<= 8;
                x |= arr[2]; x <<= 8;
                x |= arr[3]; x <<= 8;
                x |= arr[4]; x <<= 8;
                x |= arr[5]; x <<= 8;
                x |= arr[6]; x <<= 8;
                x |= arr[7];
                return true;
            }
        }


        size_t decoder:: construct64(istream &fp, uint64_t &value, const char *info)
        {

            uint8_t b[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
            size_t  nr    = read<uint8_t>(fp,b[0]);
            if(nr<=0) throw libc::exception( EIO, "no data for '%s'",info?info:yack_unknown);

            if(b[0]<=0)
            {
                //______________________________________________________________
                //
                // it's ZERO
                //______________________________________________________________
                value = 0;
                assert(1==nr);
                return 1;
            }
            else
            {
                //______________________________________________________________
                //
                // unpack!
                //______________________________________________________________
                const unsigned bytes = (b[0] >> 4) & 0xf;
                if(bytes<1||bytes>9)
                {
                    throw libc::exception(EIO,"invalid #bytes=%u for '%s'",bytes,info?info:yack_unknown);
                }

                //______________________________________________________________
                //
                // read extra bytes
                //______________________________________________________________
                {
                    const unsigned extra = bytes-1;
                    if(extra!=fp.fetch(b+1,extra)) throw libc::exception(EIO,"missing bytes for '%s'",info?info:yack_unknown);
                }
                
                //______________________________________________________________
                //
                // expand bytes into quartets
                //______________________________________________________________
#define YEXPAND(I) uint8_t(b[I] >> 0x4), uint8_t(b[I] &  0xf)
                const uint8_t q[32] = {
                    uint8_t(b[0]&0xf),
                    YEXPAND(1), YEXPAND(2),YEXPAND(3),YEXPAND(4),
                    YEXPAND(5), YEXPAND(6),YEXPAND(7),YEXPAND(8),
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                };

                
                //______________________________________________________________
                //
                // build from quartets
                //______________________________________________________________
                uint64_t qw = 0;
                for(int i=15;i>=0;--i)
                {
                    qw <<= 4;
                    qw |= q[i];
                }
                value = qw;
                return bytes;
            }

        }

        void decoder:: check_bits(const uint64_t x, const size_t max_bits, const char *info)
        {
            const size_t bits = bits_for(x);
            if(bits>max_bits)
            {
                throw libc::exception(EINVAL,"too many read bits for '%s'",info?info:yack_unknown);
            }
        }

    }

}


