


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
        
        static inline
        const char *gz_wmode(const int level)
        {
            static const char *_[] =
            {
                NULL,
                "w1",
                "w2",
                "w3",
                "w4",
                "w5",
                "w6",
                "w7",
                "w8",
                "w9"
            };
            
            if(level>=1&&level<=9) return _[level];
            throw libc::exception(EINVAL,"gz::ostream invalid level=%d",level);
        }
        
        
        ogzstream:: ogzstream(const char *filename,
                              const int   level) :
        ostream(),
        gzstream(filename,gz_wmode(level))
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
