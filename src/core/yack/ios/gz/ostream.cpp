


#include "yack/ios/gz/ostream.hpp"
#include "../../../../zlib/zlib.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace ios
    {
        ogzstream:: ~ogzstream() throw()
        {
        }
        
        ogzstream:: ogzstream(const char *filename) :
        ostream(),
        gzstream(filename,"w6")
        {
        }
        
        void ogzstream:: flush()
        {
            
        }
        
        void ogzstream:: write(const char C)
        {
            if( gzputc( static_cast<gzFile>(GZ), C)  < 0 )
            {
                throw libc::exception(EIO,"gzputc");
            }
        }
        
    }
    
}
