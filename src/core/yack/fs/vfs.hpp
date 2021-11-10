
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    class vfs
    {
    public:
        static const char *get_base_name(const char *path) throw();


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vfs);
    };



}

#endif
