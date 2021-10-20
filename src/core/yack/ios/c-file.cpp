#include "yack/ios/c-file.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"

#include <cstdio>
#include <cerrno>

namespace yack
{
    namespace ios
    {

        const cstdin_t  cstdin  = {};
        const cstdout_t cstdout = {};
        const cstderr_t cstderr = {};


        system_error_t c_file::last_close_error = 0;
        
        c_file:: ~c_file() throw()
        {
            assert(handle);
            if(_close)
            {
                YACK_GIANT_LOCK();
                last_close_error = 0;
                if( 0 != fclose( (FILE*)handle ) )
                {
                    last_close_error = errno;
                }
            }
            handle = 0;
        }


        c_file:: c_file(const cstdin_t &) :
        handle( stdin ),
        _close( false )
        {
            if(!handle) throw libc::exception(EINVAL,"ios::stdin is NULL");
        }

        c_file:: c_file(const cstdout_t &) :
        handle( stdout ),
        _close( false  )
        {
            if(!handle) throw libc::exception(EINVAL,"ios::stdout is NULL");
        }


        c_file:: c_file(const cstderr_t &) :
        handle( stderr ),
        _close( false  )
        {
            if(!handle) throw libc::exception(EINVAL,"ios::stderr is NULL");
        }

        c_file:: c_file(const char *filename, mode_type m) :
        handle( _open(filename,m) ),
        _close( true )
        {

        }

        static inline const char *mode2text(const c_file::mode_type m)
        {
            switch(m)
            {
                case c_file::r: return "rb";
                case c_file::w: return "wb";
                case c_file::a: return "ab";
            }
            system_error::critical_bsd(EINVAL,"c_file::mode_type");
            return NULL;
        }

        void * c_file:: _open(const char *filename, mode_type m)
        {
            assert(NULL!=filename);
            YACK_GIANT_LOCK();
            const char *tx = mode2text(m);
            FILE       *fp = fopen(filename,tx);
            if(!fp) throw libc::exception(errno,"fopen(%s,%s)",filename,tx);
            return fp;
        }


    }

}

#include <cstdarg>

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

        writable_file:: writable_file(const char *filename, const bool append) :
        c_file(filename,append?a:w)
        {

        }

        void writable_file:: put(const char C)
        {
            YACK_GIANT_LOCK();
            assert(handle);
            if(EOF==fputc(C, static_cast<FILE*>(handle)))
            {
                throw libc::exception(errno,"fputc(%c)",C);
            }

        }

        void writable_file:: put(const char *fmt,void *args)
        {
            assert(NULL!=fmt);
            assert(NULL!=args);
            va_list &ap = *static_cast<va_list *>(args);
            try
            {
                if(vfprintf(static_cast<FILE *>(handle),fmt,ap)<0)
                {
                    throw libc::exception(errno,"vfprintf(...)");
                }
            }
            catch(...)
            {
                va_end(ap);
                throw;
            }

        }

        void writable_file:: putf(const char *fmt,...)
        {
            assert(NULL!=fmt);
            va_list ap;
            va_start(ap,fmt);
            put(fmt,&ap);
            va_end(ap);
        }

    }

}
