
#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"

namespace yack
{

    const char *vfs:: get_base_name(const char *path) throw()
    {
        const size_t plen = yack_cstring_size(path);
        if(plen<=0)
        {
            return path;
        }
        else
        {
            return path;
        }
    }
}


