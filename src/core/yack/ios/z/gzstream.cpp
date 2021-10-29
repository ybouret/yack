
#include "yack/ios/z/gzstream.hpp"
#include "../../../../zlib/zlib.h"

namespace yack
{
    namespace ios
    {
        
        gzstream:: ~gzstream() throw()
        {
            assert(GZ);
            gzclose( static_cast<gzFile>(GZ) );
            GZ=0;
        }
        
    }
    
}


