#include "yack/ios/c/writable.hpp"
#include "yack/lockable.hpp"
#include "yack/system/exception.hpp"
#include <cstdarg>
#include <cstdio>
#include <cerrno>

namespace yack
{
    namespace ios
    {
        writable_file:: writable_file(const cstderr_t &_) :
        c_file(_)
        {
        }

        writable_file:: writable_file(const cstdout_t &_) :
        c_file(_)
        {
        }

        writable_file:: ~writable_file() throw()
        {
        }

        writable_file:: writable_file(const char *filename, const bool append) : c_file(filename,append?a:w) {}

        writable_file:: writable_file(const string &filename, const bool append) : c_file(filename,append?a:w) {}


        void writable_file:: put(const char C)
        {
            YACK_GIANT_LOCK();
            assert(handle);
            if(EOF==fputc(C, static_cast<FILE*>(handle)))
            {
                throw libc::exception(errno,"fputc(%c)",C);
            }

        }

        size_t writable_file:: put(const char *fmt,void *args)
        {
            assert(NULL!=fmt);
            assert(NULL!=args);
            va_list &ap = *static_cast<va_list *>(args);
            try
            {
                YACK_GIANT_LOCK();
                const int res = vfprintf(static_cast<FILE *>(handle),fmt,ap);
                if(res<0)
                {
                    throw libc::exception(errno,"vfprintf(...)");
                }
                return res;
            }
            catch(...)
            {
                va_end(ap);
                throw;
            }

        }

        size_t writable_file:: putf(const char *fmt,...)
        {
            assert(NULL!=fmt);
            va_list ap;
            va_start(ap,fmt);
            const size_t nw = put(fmt,&ap);
            va_end(ap);
            return nw;
        }

        void writable_file::put(const void *addr,const size_t size)
        {
            assert(!(NULL==addr&&size>0));
            YACK_GIANT_LOCK();
            if(fwrite(addr,1,size,static_cast<FILE*>(handle))<size)
            {
                throw libc::exception(errno,"writable_file::put(%lu)", (unsigned long)size );
            }
        }

    }

}
