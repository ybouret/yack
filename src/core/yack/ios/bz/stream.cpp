

#include "yack/ios/bz/stream.hpp"
#include "yack/bzip2/bzlib.h"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <cerrno>
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {
        namespace bz
        {
            stream:: ~stream() throw()
            {
                assert(NULL==BZ);
            }




            stream:: stream(void *bz) throw() :
            BZ( bz )
            {
                assert(NULL!=BZ);
            }


    }

}


