#include "yack/ios/bz/istream.hpp"
#include "yack/bzip2/bzlib.h"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <cerrno>
#include "yack/string.hpp"
#include <cstdio>

namespace yack
{
    namespace ios
    {
        namespace bz
        {
            void * istream::open_stream(void *handle)
            {
                assert(handle);
                YACK_GIANT_LOCK();
                int     err = BZ_OK;
                BZFILE *ptr = BZ2_bzReadOpen(&err,(FILE*)handle,0,0,NULL,0);
                if(BZ_OK!=err)
                {
                    BZ2_bzReadClose(&err,ptr);
                    throw exception("%s in %s", bz::stream::errstr(err), istream::clid);
                }
                return ptr;
            }

            const char istream:: clid[] = "bz::istream";

            istream:: ~istream() throw()
            {
                assert(handle);
                int     err = BZ_OK;
                BZ2_bzReadClose(&err,static_cast<BZFILE*>(BZ));
                BZ = 0;
            }

            size_t istream:: fetch_(void *addr, const size_t size)
            {
                assert(BZ);
                assert(yack_good(addr,size));
                if(eos) return 0;
                YACK_GIANT_LOCK();
                int err = BZ_OK;
                int res = BZ2_bzRead(&err, static_cast<BZFILE *>(BZ),addr,int(size));
                switch(err)
                {
                    case BZ_STREAM_END: eos=true; // FALLTHRU
                    case BZ_OK:
                        return res;

                    default:
                        break;
                }
                throw exception("BZ2_bzRead: %s", errstr(err) );
            }

            bool istream:: query_(char &C)
            {
                return fetch_(&C,1)>0;
            }

        }

    }

}

