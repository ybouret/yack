#include "yack/ios/c/file.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>

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

        static void throw_on_null(const char *id)
        {
            assert(id);
            libc::exception(EINVAL,"%s is NULL",id);
        }

        c_file:: c_file(const cstdin_t &) :
        _close( false ),
        handle( stdin )
        {
            if(!handle) throw_on_null("ios::stdin");
        }

        c_file:: c_file(const cstdout_t &) :
        _close( false  ),
        handle( stdout )
        {
            if(!handle) throw_on_null("ios::stdout");
        }


        c_file:: c_file(const cstderr_t &) :
        _close( false  ),
        handle( stderr )
        {
            if(!handle) throw_on_null("ios::stderr");
        }

        c_file:: c_file(const char *filename, mode_type m) :
        _close( true ),
        handle( _open(filename,m,coerce(_close)) )
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

        void * c_file:: _open(const char *filename, mode_type m, bool &auto_close)
        {
            // default parameters
            assert(NULL!=filename);
            assert(true==auto_close);

            YACK_GIANT_LOCK();

            switch(m)
            {
                case r:
                    if( 0 == strcmp(filename,YACK_STDIN) )
                    {
                        if(NULL==stdin) throw_on_null(filename);
                        auto_close = false;
                        return stdin;
                    }
                    break;


                case w:
                case a:
                    if( 0 == strcmp(filename,YACK_STDOUT) )
                    {
                        if(NULL==stdout)  throw_on_null(filename);
                        auto_close = false;
                        return stdout;
                    }

                    if( 0 == strcmp(filename,YACK_STDERR) )
                    {
                        if(NULL==stderr) throw_on_null(filename);
                        auto_close = false;
                        return stderr;
                    }
                    break;
            }

            // default file
            const char *tx = mode2text(m);
            FILE       *fp = fopen(filename,tx);
            if(!fp) throw libc::exception(errno,"fopen(%s,%s)",filename,tx);
            return fp;

        }

        void c_file:: flush()
        {
            fflush( (FILE*)handle );
        }


    }

}

