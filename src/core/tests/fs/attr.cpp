

#include "yack/fs/local/fs.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <dirent.h>
#include <cerrno>
#endif

using namespace yack;

YACK_UTEST(vfs_attr)
{
    localFS &fs = localFS::instance();
    for(int i=1;i<argc;++i)
    {
        const string             path = argv[i];
        const vfs::entry         ep(fs,path);
        std::cerr << "[" << path << "]";
        if(ep.is_unk()) std::cerr << " unknown   |";
        if(ep.is_dir()) std::cerr << " directory |";
        if(ep.is_reg()) std::cerr << " regular   |";
        if(ep.is_lnk()) std::cerr << " symbolic  |";
        std::cerr << std::endl;
    }


}
YACK_UDONE()


