#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include <cstring>

namespace yack
{
    vfs:: entry:: ~entry() throw()
    {
    }

    vfs::entry:: entry(const vfs    &root,
                       const string &here) :
    object(),
    next(0),
    prev(0),
    path( new string(here)         ),
    base(vfs::get_base_name(*path) ),
    cext(vfs::get_extension(base)  ),
    attr( root.get_attr_of(*path)  )
    {
    }

    bool vfs::entry:: is_unk() const throw()
    {
        return 0  == (attr&attr_msk);
    }

    bool vfs::entry:: is_reg() const throw()
    {
        return 0 !=  (attr&attr_reg);
    }

    bool vfs::entry:: is_dir() const throw()
    {
        return 0 !=  (attr&attr_dir);
    }

    bool vfs::entry:: is_lnk() const throw()
    {
        return  0 != (attr_lnk&attr);
    }

    vfs::scanner:: ~scanner() throw() {}
    vfs::scanner::  scanner(const vfs    &fs,
                            const string &dirname) :
    object(),
    root(fs),
    path(dirname)
    {
        make_dir_name( coerce(path) );
    }


}

