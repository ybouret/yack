
#include "yack/string/utf8.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

#include <iostream>

namespace yack
{
    const char utf8:: clid[] = "UTF-8";
    
    
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
                        data[3] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[2] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[1] = bit7                  | uint8_t(code&mask6); code >>= 6;
                        data[0] = (bit7|bit6|bit5|bit4) | uint8_t(code);
                        return 4;
                    }
                    else
                    {
                        throw libc::exception(EINVAL,"%s invalid code point=0x%08x",clid,code);
                    }
                }
            }
        }
    }
    
}

namespace yack
{
    utf8::decoding utf8::decode_init(uint32_t &code, const uint8_t data)
    {
        if(data<=0x7f)
        {
            code = data;
            return decoding_done;
        }
        
        if(data<0xC2) goto ERROR;
        
        if(data<=0xDF)
        {
            assert(data>=0xC2);
            code = uint32_t(data & 31) << 6;
            return decoding_wait1;
        }
        
        if(data<=0xEF)
        {
            assert(data>=0xE0);
            code = uint32_t(data & 15) << 6;
            return decoding_wait2;
        }
        
        if(data<=0xF4)
        {
            assert(data>=0xF0);
            code = uint32_t(data & 7) << 6;
            return decoding_wait3;
        }
         
 
    ERROR:
        throw libc::exception(EINVAL,"%s invalid first byte 0x%02x",clid,data);
    }

    utf8::decoding utf8::decode_next(uint32_t      &code,
                                     const uint8_t  data,
                                     const decoding flag)
    {
        assert(0==(code&63));
        
        switch(flag)
        {
            case decoding_wait1:
                code |= decode6bits(data);
                return decoding_done;
         
            case decoding_wait2:
                code |= decode6bits(data);
                code <<= 6;
                return decoding_wait1;
                
            case decoding_wait3:
                code |= decode6bits(data);
                code <<= 6;
                return decoding_wait2;
                
            default:
                break;
        }
        
        throw exception("%s corrupted decoder",clid);
    }
    
    uint8_t  utf8::decode6bits(const uint8_t data)
    {
        if(data>=0x80&&data<=0xBF)
        {
            return (data&63);
        }
        throw libc::exception(EINVAL,"%s invalid coding byte 0x%02x",clid,data);
    }

    uint32_t utf8::decode(const uint8_t data[], const size_t size)
    {
        assert(data!=NULL);
        assert(size>0);
        assert(size<=4);
        uint32_t code = 0;
        const uint8_t *addr = data;
        size_t         left = size;
        decoding       flag = decode_init(code,*addr);
    DECODE:
        assert(left>0);
        ++addr;
        --left;
        switch(flag)
        {
            case decoding_done:
                if(left>0) break;
                return code;
                
            case decoding_wait1:
                if(left!=1) break;
                flag = decode_next(code,*addr,flag);
                goto DECODE;
                
            case decoding_wait2:
                if(left!=2) break;
                flag = decode_next(code,*addr,flag);
                goto DECODE;
                
            case decoding_wait3:
                if(left!=3) break;
                flag = decode_next(code,*addr,flag);
                goto DECODE;
        }
        throw libc::exception(EINVAL,"%s invalid sequence",clid);
    }

    
}
