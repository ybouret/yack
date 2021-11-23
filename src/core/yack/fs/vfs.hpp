
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    //! Virtual File System interface
    class vfs
    {
    public:
        static const char *get_base_name(const char *path, const size_t plen) throw(); //!< get base name
        static const char *get_base_name(const char *path) throw();                    //!< get base name
        static const char *get_base_name(const string &path) throw();                  //!< get base name

        static const char *get_extension(const char   *path, const size_t plen) throw(); //!< get extension, NULL if none
        static const char *get_extension(const char   *path)                    throw(); //!< get extension
        static const char *get_extension(const string &path) throw();                    //!< get extension

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vfs);
    };



}

#endif
