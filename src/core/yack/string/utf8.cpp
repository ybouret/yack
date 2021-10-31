
#include "yack/string/utf8.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    
    
    
    size_t utf8:: encode(uint8_t data[], uint32_t code)
    {
        if(code<=0x7f)
        {
            // 0-7 bits
            data[0] = uint8_t(code);
            return 1;
        }
        else
        {
            assert(code>=0x80);
            static const uint32_t mask6 = 1|2|4|8|16|32;
            static const uint8_t  bit7  = 128;
            static const uint8_t  bit6  = 64;
            if(code<=0x07ff)
            {
                // 8-11 : 5 + 6
                data[1] = bit7        | uint8_t(code&mask6); code >>= 6;
                data[0] = (bit7|bit6) | uint8_t(code);
                return 2;
            }
            else
            {
                assert(code>=0x0800);
                static const uint8_t bit5 = 32;
                if(code<=0xffff)
                {
                    // 12 - 16 : 4 + 6 + 6
                    data[2] = bit7             | uint8_t(code&mask6); code >>= 6;
                    data[1] = bit7             | uint8_t(code&mask6); code >>= 6;
                    data[0] = (bit7|bit6|bit5) | uint8_t(code);
                    return 3;
                }
                else
                {
                    assert(code>=0x10000);
                    if(code<=0x10FFFF)
                    {
                        static const uint8_t bit4 = 16;
                        // 17-21: 3+6+6+6
                        data[2] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[2] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[1] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[0] = (bit7|bit6|bit5|bit4) | uint8_t(code);
                        return 4;
                    }
                    else
                    {
                        throw libc::exception(EINVAL,"invalid utf8=0x%08x",code);
                    }
                }
            }
        }
    }
    
}
