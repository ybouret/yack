

#include "yack/ios/pstream.hpp"
#include "yack/lockable.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"
#include <cstdio>
#include <cerrno>

namespace yack
{
    namespace ios
    {

        pstream:: ~pstream() throw()
        {
            assert(handle);
#if defined(YACK_BSD)
            *retval = pclose( (FILE*) handle );
#endif

#if defined(YACK_WIN)
            *retval = _pclose( (FILE*) handle );
#endif
            handle = NULL;
        }

        void   * pstream:: init(const string &cmd)
        {
            YACK_GIANT_LOCK();
#if defined(YACK_BSD)
            FILE *fp = popen(cmd(), "r+" );
            if(!fp) throw libc::exception(errno,"popen('%s')",cmd());
            return fp;
#endif

#if defined(YACK_WIN)
            FILE *fp = _popen(cmd(), "r" );
            if(!fp) throw libc::exception(errno,"popen('%s')",cmd());
            return fp;
#endif
        }

        void * pstream:: init(const char *cmd)
        {
            const string _(cmd);
            return init(_);
        }

        bool pstream:: query_(char &C)
        {
            YACK_GIANT_LOCK();
            FILE        *fp = static_cast<FILE *>(handle);
            const size_t nr = fread(&C,1,1,fp);
            if(nr<=0)
            {
                if(feof(fp)) return false;
                throw libc::exception(errno,"ios::pstream::query(char)");
            }
            else
            {
                return true;
            }
        }

        size_t pstream:: fetch_(void *addr, const size_t size)
        {
            YACK_GIANT_LOCK();
            assert(addr!=NULL);
            assert(size>0);
            FILE        *fp = static_cast<FILE *>(handle);
            const size_t nr = fread(addr,1,size,fp);
            if(nr<size && ferror(fp))
                throw libc::exception(errno,"ios::pstream::fetch(%lu)",(unsigned long)size);
            return nr;
        }

        void pstream:: load(sequence<string> &lines)
        {
            characters line;
            while( gets(line) )
            {
                {
                    const string empty;
                    lines.push_back(empty);
                }

                string s = line.to_string();
                s.xch(lines.back());
            }
        }

        void  pstream:: check(const result &res, const string &cmd)
        {
            if( 0 != *res ) throw libc::exception( *res, "pstream('%s')", cmd() );
        }

        void  pstream:: check(const result &res, const char   *cmd)
        {
            const string _(cmd);
            check(res,_);
        }


    }
}

