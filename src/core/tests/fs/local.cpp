
#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <dirent.h>
#include <cerrno>
#endif

using namespace yack;

YACK_UTEST(vfs_local)
{
    local_fs &fs = local_fs::instance();
    std::cerr << fs.call_sign << std::endl;
    if(argc>1)
    {
        const string dname = argv[1];
        std::cerr << "opening " << dname << std::endl;
#if defined(YACK_BSD)
        DIR *hdir = opendir( dname() );
        if(!hdir)
        {
            throw libc::exception(errno,"opendir(%s)", dname() );
        }
        dirent *scan = NULL;
        while( NULL !=(scan=readdir(hdir)) )
        {
            std::cerr << scan->d_name << " | " << int(scan->d_type) << std::endl;
        }

        closedir(hdir);
#endif

    }

}
YACK_UDONE()


