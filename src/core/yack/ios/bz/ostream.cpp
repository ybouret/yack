
#include "yack/ios/bz/ostream.hpp"
#include "yack/bzip2/bzlib.h"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <cerrno>
#include "yack/string.hpp"
#include <cstdio>
#include "yack/type/utils.hpp"

namespace yack
{
    namespace ios
    {
        namespace bz
        {

            void * ostream:: open_stream(void *fp, const int level)
            {
                assert(NULL!=fp);
                YACK_GIANT_LOCK();

                int     err = BZ_OK;
                BZFILE *ptr = BZ2_bzWriteOpen(&err,(FILE*)fp, clamp<int>(0,level,9), 0, 0);
                if(BZ_OK!=err)
                {
                    BZ2_bzWriteClose(&err,ptr,1,NULL,NULL);
                    throw exception("%s in %s", stream::errstr(err), ostream::clid);
                }

                return ptr;
            }

            const char ostream:: clid[] = "bz::ostream";

            ostream:: ~ostream() throw()
            {
                // close
                YACK_GIANT_LOCK();
                int     err = BZ_OK;
                BZFILE *ptr = (BZFILE*)BZ;
                BZ2_bzWriteClose(&err,ptr,0,NULL,NULL);
            }

            
        }
    }
}


