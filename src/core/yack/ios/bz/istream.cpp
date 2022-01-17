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

            static inline
            void * open_stream(const char *filename, void **ppf)
            {
                assert(filename);
                assert(ppf);
                YACK_GIANT_LOCK();
                FILE   *fp  = fopen(filename,"rb");  if(!fp) throw libc::exception(errno,"%s(%s)", istream::clid, filename);
                int     err = BZ_OK;
                BZFILE *ptr = BZ2_bzReadOpen(&err,fp,0,0,NULL,0);
                if(BZ_OK!=err)
                {
                    BZ2_bzReadClose(&err,ptr);
                    fclose(fp);
                    throw exception("%s in %s(%s)", bz::stream::errstr(err), istream::clid, filename);
                }
                *ppf = fp;
                return ptr;
            }

            const char istream:: clid[] = "bz::istream";

            istream:: ~istream() throw()
            {
                int     err = BZ_OK;
                BZ2_bzReadClose(&err,static_cast<BZFILE*>(BZ));
                BZ = 0;
            }

            istream:: istream(const string &filename) :
            ios::istream(),
            stream(),
            eos(false)
            {
                BZ = open_stream(filename(),&fp);
            }

            istream:: istream(const char *filename) :
            ios::istream(),
            stream( ),
            eos(false)
            {
                BZ = open_stream(filename,&fp);
            }

            size_t istream:: fetch_(void *addr, const size_t size)
            {
                assert(BZ);
                assert(yack_good(addr,size));
                if(eos) return 0;
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

