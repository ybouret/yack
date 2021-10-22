#include "yack/ios/c/file.hpp"
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

        void c_file:: flush()
        {
            fflush( (FILE*)handle );
        }


    }

}

