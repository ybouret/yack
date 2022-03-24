
//! \file

#ifndef YACK_STRING_FWD_INCLUDED
#define YACK_STRING_FWD_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace kernel
    {
        template <typename T>
        class string;
    }

    typedef kernel::string<char> string;
}

#endif

