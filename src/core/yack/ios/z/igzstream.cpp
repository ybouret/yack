

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
        
        
    }
    
}
