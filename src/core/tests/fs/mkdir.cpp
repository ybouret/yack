


#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <dirent.h>
#include <cerrno>
#endif

using namespace yack;

YACK_UTEST(vfs_mkdir)
{
    localFS &fs = localFS::instance();
    if(argc>1)
    {
        const string path = argv[1];
        fs.create_subdirectory(path);
    }


}
YACK_UDONE()


