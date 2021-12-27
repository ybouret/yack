
//! \file

#ifndef YACK_CXX_NAME_INCLUDED
#define YACK_CXX_NAME_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    struct cxx_name
    {
        static string of(const string &);
        static string of(const char   *);
    };

}

#endif
