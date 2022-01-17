
#include "yack/ios/bz/ostream.hpp"
#include "yack/bzip2/bzlib.h"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <cerrno>
#include "yack/string.hpp"
#include <cstdio>
#include "yack/type/utils.hpp"
#include "yack/type/ints.hpp"

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
                YACK_GIANT_LOCK();
                int     err = BZ_OK;
                BZFILE *ptr = (BZFILE*)BZ;
                BZ2_bzWriteClose(&err,ptr,0,NULL,NULL);
                BZ=0;
            }


            void ostream:: output(const void *addr, const size_t size)
            {
                assert( yack_good(addr,size) );
                static const size_t szmax = integral_for<int>::maximum;
                if(size>szmax) throw libc::exception(EIO,"%s max output size exceeded",clid);
                int     err = BZ_OK;
                BZFILE *ptr = (BZFILE*)BZ;
                BZ2_bzWrite(&err,ptr,(void*)addr,(int)size);
                if(BZ_OK!=err)
                {
                    throw exception("%s in %s::output", stream::errstr(err), ostream::clid);
                }
            }

            void ostream:: frame(const void *addr, const size_t size)
            {
                output(addr,size);
            }

            void ostream:: flush()
            {

            }

            void ostream:: write(char C)
            {
                output(&C,1);
            }
            
        }
    }
}


