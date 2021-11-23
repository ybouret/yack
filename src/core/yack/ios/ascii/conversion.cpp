#include "yack/ios/ascii/conversion.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{

    namespace ios
    {

        namespace ascii
        {
            int64_t convert::to_int64(const char *args,   size_t size, const char *ctx)
            {
                static const char fn[] = "ascii.convert.to_int";
                assert(!(NULL==args&&size>0));
                if(size<=0) throw libc::exception(EINVAL,"%s empty argument for %s",fn,ctx?ctx:yack_unknown);
                bool    neg=false;
                int64_t res=0;

                switch(args[0])
                {
                    case '-':
                        neg = true;
                    case '+':
                        ++args;
                        --size;
                        if(size<=0) throw libc::exception(EINVAL,"%s empty argument after '-' for %s",fn,ctx?ctx:yack_unknown);
                        break;
                    default:
                        break;
                }

                assert(size>0);
                while(size-- > 0)
                {
                    const char c = *(args++);
                    res *= 10;
                    switch(c)
                    {
                        case '0': break;
                        case '1': ++res;    break;
                        case '2': res += 2; break;
                        case '3': res += 3; break;
                        case '4': res += 4; break;
                        case '5': res += 5; break;
                        case '6': res += 6; break;
                        case '7': res += 7; break;
                        case '8': res += 8; break;
                        case '9': res += 9; break;
                        default:
                            throw libc::exception(EINVAL,"%s invalid char '%s' for %s",fn, hybrid[uint8_t(c)], ctx?ctx:yack_unknown);
                    }
                    
                }

                return res;
            }

        }
    }
}


