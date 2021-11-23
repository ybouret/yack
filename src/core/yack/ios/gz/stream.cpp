
#include "yack/ios/gz/stream.hpp"
#include "../../../../zlib/zlib.h"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <cerrno>
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {
        namespace gz
        {
            stream:: ~stream() throw()
            {
                assert(GZ);
                gzclose( static_cast<gzFile>(GZ) );
                GZ=0;
            }

            static inline
            void *open_gzstream(const char *filename, const char *mode)
            {
                assert(NULL!=filename);
                assert(NULL!=mode);
                YACK_GIANT_LOCK();
                gzFile fp = gzopen(filename,mode);
                if(!fp)
                {
                    throw libc::exception(errno,"gzopen(%s,%s)",filename,mode);
                }

                return fp;
            }

            stream:: stream(const char *filename, const char *mode) :
            GZ( open_gzstream(filename,mode) )
            {
            }

            stream:: stream(const string &filename, const char *mode) :
            GZ( open_gzstream(&filename[1],mode) )
            {
            }

        }
        
    }
    
}


