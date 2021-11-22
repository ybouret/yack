

#include "yack/string/utf8.hpp"
#include "yack/system/exception.hpp"
#include "yack/system/error.hpp"
#include <cerrno>

#include <iostream>

namespace yack
{
    const char utf8:: clid[] = "UTF-8";
    
    
    
    const utf8::range<uint32_t> utf8::code_bank[num_banks] =
    {
        { 0x00000000, 0x0000007f },
        { 0x00000080, 0x000007ff },
        { 0x00000800, 0x0000FFFF },
        { 0x00010000, 0x0010FFFF }
    };
    
    const utf8::range<uint8_t> utf8::byte_bank[num_banks] =
    {
        { 0x00, 0x7f },
        { 0xc2, 0xdf },
        { 0xe0, 0xef },
        { 0xf0, 0xf4 }
    };
    
    const utf8:: range<uint8_t> utf8::continuation = { 0x80, 0xbf };
    
    void utf8:: validate(uint32_t &codepoint)
    {
        for(size_t i=0,n=1;i<num_banks;++i,++n)
        {
            if(code_bank[i].owns(codepoint))
            {
                codepoint |= (n<<info_move);
                return;
            }
        }
        const uint32_t temp = codepoint;
        codepoint = 0;
        throw libc::exception(EINVAL,"%s invalid codepoint U+%x",clid,temp);
    }
    
    utf8:: utf8(const uint32_t codepoint) :
    code(codepoint)
    {
        validate(code);
    }
    
    utf8:: ~utf8() throw()
    {
        code=0;
    }
    
    utf8:: utf8(const utf8 &U) throw() : code(U.code)
    {
    }
    
    utf8 & utf8::operator=(const utf8 &U) throw()
    {
        code=U.code;
        return *this;
    }
    
    utf8 & utf8::operator=(uint32_t codepoint)
    {
        validate(codepoint);
        code=codepoint;
        return *this;
    }
  
    size_t utf8:: bytes() const throw()
    {
        const size_t res = (code>>info_move);
        assert(res>=1); assert(res<=4);
        return res;
    }

    uint32_t utf8:: operator*() const throw()
    {
        return (code&code_mask);
    }

    void utf8:: xch(utf8 &U) throw()
    {
        cswap(code,U.code);
    }

    int utf8::compare(const utf8 &lhs, const utf8 &rhs) throw()
    {
        const uint32_t L = *lhs;
        const uint32_t R = *rhs;
        return (L<R) ? -1 : ( (R<L) ? 1 : 0 );
    }



    size_t utf8:: encode(uint8_t *data) const throw()
    {
        static const uint32_t msk6 = 1|2|4|8|16|32;
        static const uint32_t bit7 = 128;
        static const uint32_t bit6 = 64;
        static const uint32_t bit5 = 32;
        static const uint32_t bit4 = 16;

        assert(data);
        uint32_t qw = **this;
        switch( bytes() )
        {
            case 1: // 0-7 bits
                data[0] = uint8_t(qw);
                return 1;

            case 2: // 8-11 bits: 5 + 6
                data[1] = bit7        | uint8_t(qw&msk6); qw >>= 6;
                data[0] = (bit7|bit6) | uint8_t(qw);
                return 2;

            case 3:  // 12 - 16 : 4 + 6 + 6
                data[2] = bit7             | uint8_t(qw&msk6); qw >>= 6;
                data[1] = bit7             | uint8_t(qw&msk6); qw >>= 6;
                data[0] = (bit7|bit6|bit5) | uint8_t(qw);
                return 3;

            case 4:
                // 17-21: 3+6+6+6
                data[3] = bit7                  | uint8_t(qw&msk6); qw >>= 6;
                data[2] = bit7                  | uint8_t(qw&msk6); qw >>= 6;
                data[1] = bit7                  | uint8_t(qw&msk6); qw >>= 6;
                data[0] = (bit7|bit6|bit5|bit4) | uint8_t(qw);
                return 4;
        }
        return 0;
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

    static inline uint8_t  decode6bits(const uint8_t data)
    {
        if(data>=0x80&&data<=0xBF)
        {
            return (data&63);
        }
        throw libc::exception(EINVAL,"%s invalid coding byte 0x%02x",utf8::clid,data);
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
        // never get here
        throw exception("%s corrupted decode_next",clid);
    }



    utf8 utf8::decode(const uint8_t data[], const size_t size)
    {
        assert(data!=NULL);
        assert(size>0);
        assert(size<=4);
        uint32_t       code = 0;
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
                return utf8(code);

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

#include <iostream>

namespace yack
{
    std::ostream & operator<<(std::ostream &os, const utf8 &u)
    {
        uint8_t      data[4] = { 0,0,0,0};
        const size_t nout    = u.encode(data);
        for(size_t i=0;i<nout;++i)
        {
            os << char(data[i]);
        }
        return os;
    }
}

#include "yack/ios/ostream.hpp"

namespace yack
{
    ios::ostream & operator<<(ios::ostream &os, const utf8 &u)
    {
        uint8_t data[4] = { 0,0,0,0};
        os.frame(data,u.encode(data));
        return os;
    }
}
