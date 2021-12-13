#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include <cstring>

namespace yack
{
    vfs:: entry:: ~entry() throw()
    {
    }

    vfs::entry:: entry(const string &full_path) :
    object(),
    next(0),
    prev(0),
    path(full_path),
    base(vfs::get_base_name(path)),
    cext(vfs::get_extension(base))
    {
    }

    vfs::scanner:: ~scanner() throw() {}
    vfs::scanner:: scanner(const string &dirname) : object(), path(dirname)
    {
        make_dir_name( coerce(path) );
    }
    

}

