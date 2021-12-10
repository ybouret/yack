#include "yack/fs/local/fs.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <unistd.h>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace yack
{

    const char local_fs::call_sign[] = "local_fs";

    local_fs:: local_fs() throw() : vfs(), singleton<local_fs>()
    {
    }

    local_fs:: ~local_fs() throw()
    {
    }


    void local_fs:: remove_file(const string &path)
    {
        YACK_GIANT_LOCK();
#if defined(YACK_BSD)
        if( unlink(path()) != 0)
        {
            throw libc::exception(errno,"unlink(%s)",path());
        }
#endif

#if defined(YACK_WIN)
        if( ! ::DeleteFile(path()) )
        {
            throw win32::exception( ::GetLastError(), "DeleteFile(%s)", path() );
        }
#endif

    }

}
