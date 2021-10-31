
#include "yack/string/utf8.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace
    {
        static const uint8_t bit[8] = {1,2,4,8,16,32,64,128};
    }
    
    utf8::status utf8::query(uint8_t &data, uint8_t &bits, const uint8_t u)
    {
        
        if(u<bit[7])
        {
            data = u;
            bits = 8;
            return header1;
        }
        else
        {
            const uint8_t header2_mask = bit[7]|bit[6]|bit[5];
            const uint8_t header2_byte = bit[7]|bit[6];
            if(header2_byte==(u&header2_mask))
            {
                static const uint8_t content_mask = bit[4]|bit[3]|bit[2]|bit[1]|bit[0];
                data = (u&content_mask);
                bits = 4;
                return header2;
            }
            else
            {
                static const uint8_t header3_mask = bit[7]|bit[6]|bit[5]|bit[4];
                static const uint8_t header3_byte = bit[7]|bit[6]|bit[5];
                if(header3_byte==(u&header3_mask))
                {
                    static const uint8_t content_mask = bit[3]|bit[2]|bit[1]|bit[0];
                    data = (u&content_mask);
                    bits = 4;
                    return header3;
                }
                else
                {
                    static const uint8_t header4_mask = bit[7]|bit[6]|bit[5]|bit[4]|bit[3];
                    static const uint8_t header4_byte = bit[7]|bit[6]|bit[5]|bit[4];
                    if(header4_byte==(u&header4_mask))
                    {
                        static const uint8_t content_mask = bit[2]|bit[1]|bit[0];
                        data = (u&content_mask);
                        bits = 3;
                        return header4;
                    }
                    else
                    {
                        static const uint8_t headerB_mask = bit[7]|bit[6];
                        static const uint8_t headerB_byte = bit[7];
                        if(headerB_byte==(u&headerB_mask))
                        {
                            static const uint8_t content_mask = bit[5]|bit[4]|bit[3]|bit[2]|bit[1]|bit[0];
                            data = (u&content_mask);
                            bits = 6;
                            return headerB;
                        }
                        else
                        {
                            //BAD
                            throw libc::exception(EINVAL,"utf8(invalid %02x)",unsigned(u));
                        }
                    }
                }
            }
        }
    }
    
    
    size_t utf8:: encode(uint8_t data[], uint32_t code)
    {
        if(code<=0x7f)
        {
            data[0] = uint8_t(code);
            return 1;
        }
        else
        {
            assert(code>=0x80);
            static const uint32_t mask6 = bit[0]|bit[1]|bit[2]|bit[3]|bit[4]|bit[5];
            if(code<=0x07ff)
            {
                static const uint32_t mask5 = bit[0]|bit[1]|bit[2]|bit[3]|bit[4];
                data[1] = bit[7] | (code&mask6); code >>= 6;
                data[0] = (bit[7]|bit[6]) | (code&mask5);
                return 2;
            }
            else
            {
                assert(code>=0x0800);
                if(code<=0xffff)
                {
                    static const uint32_t mask4 = bit[0]|bit[1]|bit[2]|bit[3];
                    data[2] = bit[7] | (code&mask6); code >>= 6;
                    data[1] = bit[7] | (code&mask6); code >>= 6;
                    data[0] = (bit[7]|bit[6]|bit[5]) | (code&mask4);
                    return 3;
                }
                else
                {
                    assert(code>=0x10000);
                    if(code<=0x10FFFF)
                    {
                        static const uint32_t mask3 = bit[0]|bit[1]|bit[2];
                        data[3] = bit[7] | (code&mask6); code >>= 6;
                        data[2] = bit[7] | (code&mask6); code >>= 6;
                        data[1] = bit[7] | (code&mask6); code >>= 6;
                        data[0] = (bit[7]|bit[6]|bit[5]|bit[4]) | (code&mask3);
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
