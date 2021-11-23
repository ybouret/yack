
#include "yack/apex/natural.hpp"
#include "yack/type/cstring.h"
#include "yack/system/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include <cerrno>

namespace yack
{

    namespace apex
    {

        static inline uint_type chr2dec(const char c)
        {
            switch(c)
            {
                case '0': return 0;
                case '1': return 1;
                case '2': return 2;
                case '3': return 3;
                case '4': return 4;
                case '5': return 5;
                case '6': return 6;
                case '7': return 7;
                case '8': return 8;
                case '9': return 9;
                default:
                    break;
            }
            throw libc::exception(EINVAL,"natural::parse_dec('%s')",ios::ascii::hybrid[ uint8_t(c) ]);
        }

        natural natural:: parse_dec(const char *data,size_t size)
        {
            assert(!(NULL==data&&size>0));

            natural res = 0;
            if(size>0)
            {
                uint_type   _ten = 10;
                const handle ten(_ten);
                res = chr2dec(data[0]);
                while(--size>0)
                {
                    const handle tmp(res);
                    res  = mul(tmp,ten);
                    res += chr2dec(*(++data));
                }
            }
            return res;
        }

        natural natural:: parse_dec(const char *data)
        {
            return parse_dec(data,yack_cstring_size(data));
        }


        static inline uint_type chr2hex(const char c)
        {
            switch(c)
            {
                case '0': return 0;
                case '1': return 1;
                case '2': return 2;
                case '3': return 3;
                case '4': return 4;
                case '5': return 5;
                case '6': return 6;
                case '7': return 7;
                case '8': return 8;
                case '9': return 9;
                case 'a':
                case 'A': return 10;
                case 'b':
                case 'B': return 11;
                case 'c':
                case 'C': return 12;
                case 'd':
                case 'D': return 13;
                case 'e':
                case 'E': return 14;
                case 'f':
                case 'F': return 15;
                default:
                    break;
            }
            throw libc::exception(EINVAL,"natural::parse_hex('%s')",ios::ascii::hybrid[ uint8_t(c) ]);
        }

        natural natural:: parse_hex(const char *data,size_t size)
        {
            assert(!(NULL==data&&size>0));

            natural res = 0;
            if(size>0)
            {
                uint_type   _sixteen = 16;
                const handle sixteen(_sixteen);
                res = chr2hex(data[0]);
                while(--size>0)
                {
                    const handle tmp(res);
                    res  = mul(tmp,sixteen);
                    res += chr2hex(*(++data));
                }
            }
            return res;
        }

        natural natural:: parse_hex(const char *data)
        {
            return parse_hex(data,yack_cstring_size(data));
        }

        natural natural:: parse(const char *data, const size_t size)
        {
            assert(!(NULL==data&&size>0));
            if(size>2)
            {
                if(data[0]=='0'&&data[1]=='x')
                {
                    return parse_hex(data+2,size-2);
                }
                else
                {
                    return parse_dec(data,size);
                }
            }
            else
            {
                return parse_dec(data,size);
            }
        }

        natural natural:: parse(const char *data)
        {
            return parse(data,yack_cstring_size(data));
        }

    }

}

