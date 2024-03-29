#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include <cstring>

namespace yack
{
    vfs:: entry:: ~entry() noexcept
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

    const string & vfs::entry:: operator*() const noexcept
    {
        return *path;
    }

    bool vfs::entry:: is_unk() const noexcept
    {
        return 0  == (attr&attr_msk);
    }

    bool vfs::entry:: is_reg() const noexcept
    {
        return 0 !=  (attr&attr_reg);
    }

    bool vfs::entry:: is_dir() const noexcept
    {
        return 0 !=  (attr&attr_dir);
    }

    bool vfs::entry:: is_lnk() const noexcept
    {
        return  0 != (attr_lnk&attr);
    }

    vfs::scanner:: ~scanner() noexcept {}
    vfs::scanner::  scanner(const vfs    &fs,
                            const string &dirname) :
    object(),
    root(fs),
    path(dirname)
    {
        make_dir_name( coerce(path) );
    }


    std::ostream & operator<<(std::ostream &os, const vfs::entry &ep)
    {
        os << '\'' << ep.path << '\'';
        return os;
    }

}

