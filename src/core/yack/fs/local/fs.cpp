#include "yack/fs/local/fs.hpp"

namespace yack
{

    const char local_fs::call_sign[] = "local_fs";

    local_fs:: local_fs() throw() : vfs(), singleton<local_fs>()
    {
    }

    local_fs:: ~local_fs() throw()
    {
    }
    

}
