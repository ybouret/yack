#include "yack/ios/bz/stream.hpp"
#include "yack/bzip2/bzlib.h"

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
            

            stream:: stream(void *ptr) throw() :
            BZ(ptr)
            {
                assert(NULL!=BZ);
            }

#define YACK_BZ(ERR) case ERR: return #ERR

            const char * stream:: errstr(const int err) throw()
            {
                switch(err)
                {
                        YACK_BZ(BZ_OK);
                        YACK_BZ(BZ_RUN_OK);
                        YACK_BZ(BZ_FLUSH_OK);
                        YACK_BZ(BZ_FINISH_OK);
                        YACK_BZ(BZ_STREAM_END);

                        YACK_BZ(BZ_CONFIG_ERROR);
                        YACK_BZ(BZ_SEQUENCE_ERROR);
                        YACK_BZ(BZ_PARAM_ERROR);
                        YACK_BZ(BZ_MEM_ERROR);
                        YACK_BZ(BZ_DATA_ERROR);
                        YACK_BZ(BZ_DATA_ERROR_MAGIC);
                        YACK_BZ(BZ_IO_ERROR);
                        YACK_BZ(BZ_UNEXPECTED_EOF);
                        YACK_BZ(BZ_OUTBUFF_FULL);
                        
                    default:
                        break;
                }

                return yack_unknown;
            }

        }

    }


}

