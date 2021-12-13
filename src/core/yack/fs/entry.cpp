#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include <cstring>

namespace yack
{
    vfs:: entry:: ~entry() throw()
    {
    }

    vfs::entry:: entry(const char *s) :
    object(),
    next(0),
    prev(0),
    path(s),
    base( vfs::get_base_name(path) )
    {
    }

    vfs::scanner:: ~scanner() throw() {}
    vfs::scanner:: scanner(const string &dirname) : object(), path(dirname) {}
    

}

