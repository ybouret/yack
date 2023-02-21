

#include "yack/ios/gz/istream.hpp"
#include "yack/zlib/zlib.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace ios
    {

        namespace gz
        {
            istream:: ~istream() noexcept
            {
            }

            istream:: istream(const char *filename) :
            ios::istream(),
            stream(filename,"r")
            {
            }

            istream:: istream(const string &filename) :
            ios::istream(),
            stream(filename,"r")
            {
            }

            bool istream:: query_(char &C)
            {
                const int ch = gzgetc( static_cast<gzFile>(GZ) );
                if(ch<0)
                {
                    return false;
                }
                else
                {
                    C = char(ch);
                    return true;
                }
            }

            size_t istream:: fetch_(void *addr, const size_t size)
            {

                const int res = gzread(static_cast<gzFile>(GZ),addr,static_cast<unsigned int>(size) );
                if(res<0)
                {
                    throw libc::exception(EIO,"gzread");
                }
                return size_t(res);
            }
        }
    }
    
}
