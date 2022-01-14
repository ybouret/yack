
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

            static inline
            void * open_stream(const char *filename, const int level)
            {
                assert(filename);
                YACK_GIANT_LOCK();
                FILE *fp = fopen(filename,"wb");
                if(!fp)
                {
                    throw libc::exception(errno,"%s(%s)", ostream::clid, filename);
                }
                int     err = BZ_OK;
                BZFILE *ptr = BZ2_bzWriteOpen(&err,fp, level, 0, 0);
                if(BZ_OK!=err)
                {
                    BZ2_bzWriteClose(&err,ptr,1,NULL,NULL);
                    throw exception("%s in %s(%s)", stream::errstr(err), ostream::clid, filename);
                }

                return ptr;
            }

            const char ostream:: clid[] = "bz::ostream";

            ostream:: ~ostream() throw()
            {
                BZ=0;
            }

            ostream:: ostream(const string &filename, const int level) :
            ios::ostream(),
            stream( open_stream( filename(), clamp<int>(0,level,9) ) )
            {

            }

            ostream:: ostream(const char *filename, const int level) :
            ios::ostream(),
            stream( open_stream(filename, clamp<int>(0,level,9) ) )
            {

            }

        }
    }
}


