

#include "yack/ios/z/igzstream.hpp"
#include "../../../../zlib/zlib.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace ios
    {
        igzstream:: ~igzstream() throw()
        {
        }
        
        igzstream:: igzstream(const char *filename) :
        istream(),
        gzstream(filename,"r")
        {
        }
        
        bool igzstream:: query_(char &C)
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
        
        size_t igzstream:: fetch_(void *addr, const size_t size)
        {
            
            const int res = gzread(static_cast<gzFile>(GZ),addr,size);
            if(res<0)
            {
                throw libc::exception(EIO,"gzread");
            }
            return size_t(res);
        }
        
    }
    
}
